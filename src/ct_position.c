#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cJSON.h"

#include "ct_coinfile.h"
#include "ct_position.h"
#include "ct_tab.h"
#include "ct_urldata.h"

void
ct_position_error_set(ct_position_t * _self, ct_position_error_t _e)
{
  assert(_self);

  _self->error |= _e;
}

ct_position_error_t
ct_position_error_get(ct_position_t * const _self)
{
  assert(_self);

  return (ct_position_error_t) _self->error;
}

void
ct_position_update(ct_position_t * _self)
{
  assert(_self);

  ct_urldata_t * _response = ct_urldata_new(_self->symbol);

  if(_response->http_code != 200) return;

  cJSON *        _json;
  cJSON *        _price;
  cJSON *        _size;
  cJSON *        _bid;
  cJSON *        _ask;
  cJSON *        _volume;

  assert(_response);

  _json = cJSON_ParseWithLength(_response->memory, _response->size);

  if(!_json)
  {
    ct_tab_endwin();
    printf("%s\n", _response->memory);
  }

  assert(_json);

  _price = cJSON_GetObjectItemCaseSensitive(_json, "price");

  if(!_price)
  {
    printf("%ld: %s\n", _response->http_code, _response->memory);
  }
  assert(_price);

  _size = cJSON_GetObjectItemCaseSensitive(_json, "size");

  assert(_size);

  _bid = cJSON_GetObjectItemCaseSensitive(_json, "bid");

  assert(_bid);

  _ask = cJSON_GetObjectItemCaseSensitive(_json, "ask");

  assert(_ask);

  _volume = cJSON_GetObjectItemCaseSensitive(_json, "volume");

  assert(_volume);

  if(
    cJSON_IsString(_price)
    && NULL != _price->valuestring
  )
  {
    _self->price_usd = strtod(_price->valuestring, NULL);
  }

  if(
    cJSON_IsString(_size)
    && NULL != _size->valuestring
  )
  {
    _self->size = strtod(_size->valuestring, NULL);
  }

  if(
    cJSON_IsString(_bid)
    && NULL != _bid->valuestring
  )
  {
    _self->bid_usd = strtod(_bid->valuestring, NULL);
  }

  if(
    cJSON_IsString(_ask)
    && NULL != _ask->valuestring
  )
  {
    _self->ask_usd = strtod(_ask->valuestring, NULL);
  }

  if(
    cJSON_IsString(_volume)
    && NULL != _volume->valuestring
  )
  {
    _self->volume = strtod(_volume->valuestring, NULL);
  }

  if(
    _self->quantity > 0
    && _self->buy_usd > 0
  )
  {
    double _cost   = _self->buy_usd * _self->quantity
           ,_gross = _self->price_usd * _self->quantity
           ;

    _self->yield_usd = _gross - _cost;
  }

  cJSON_Delete(_json);
  ct_urldata_destroy(&_response);
}

ct_position_t *
ct_position_new(char * const _line)
{
  static int      _id    = 0;
  ct_position_t * _p     = (ct_position_t *) malloc(sizeof(ct_position_t));
  int             _field = 0;
  char *          _token = strtok(_line, CT_COINFILE_TOKEN_DELIMITERS);

  assert(_p);

  _p->id        = _id++;
  _p->raw       = strndup(_line, CT_COINFILE_LINE_WIDTH_MAX);
  _p->symbol    = NULL;
  _p->quantity  = 0;
  _p->buy_usd   = 0;
  _p->price_usd = 0;
  _p->size      = 0;
  _p->bid_usd   = 0;
  _p->ask_usd   = 0;
  _p->volume    = 0;
  _p->yield_usd = 0;
  _p->time      = NULL;
  _p->next      = NULL;

  while(_token)
  {
    switch(_field)
    {
      default:
        ct_position_error_set(_p, CT_POSITION_ERROR_FIELD_OUT_OF_RANGE);
        break;

      case CT_FILE_FIELD_SYMBOL:
        _p->symbol = strndup(_token, CT_POSITION_SYMBOL_LENGTH_MAX);
        break;

      case CT_FILE_FIELD_QUANTITY:
        _p->quantity = strtod(_token, NULL);
        break;

      case CT_FILE_FIELD_BUY_PRICE_USD:
        _p->buy_usd = strtod(_token, NULL);
        break;
    }

    _token = strtok(NULL, CT_COINFILE_TOKEN_DELIMITERS);

    _field++;
  }

  return _p;
}

void
ct_position_destroy(ct_position_t ** _self_p)
{
  assert(_self_p);

  if(*_self_p)
  {
    ct_position_t * _position = *_self_p;

    if(_position->raw)    free(_position->raw);
    if(_position->symbol) free(_position->symbol);

    free(*_self_p);
  }
}

