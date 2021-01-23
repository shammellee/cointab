#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <curl/curl.h>
// #include <libwebsockets.h>

#include "cJSON.h"

#include "ct_coinfile.h"
#include "ct_pathtools.h"
#include "ct_portfolio.h"
#include "ct_position.h"
#include "ct_tab.h"

/* static char CT_PORTFOLIO_PATH[CT_PATH_LENGTH_MAX]; */

/* sprintf(CT_PORTFOLIO_PATH, "%s/.coinrc", getenv("HOME")); */

void
ct_portfolio_error_set(ct_portfolio_t * _self, ct_portfolio_error_t _e)
{
  assert(_self);

  _self->error |= _e;
}

ct_portfolio_error_t
ct_portfolio_error_get(ct_portfolio_t * const _self)
{
  assert(_self);

  return (ct_portfolio_error_t) _self->error;
}

void
ct_portfolio_position_each(ct_portfolio_t * _self, ct_position_iterator _f)
{
  assert(_self);
  assert(_f);

  ct_position_t * _p         = _self->head;
  double          _total_usd = 0;

  while(_p)
  {
    _f(_p);

    _total_usd += _p->yield_usd;

    _p = _p->next;
  }

  _self->holdings_total_usd = _total_usd;
}

ct_portfolio_t *
ct_portfolio_new(const char * const _path)
{
  char *  _path_safe;
  FILE *  _file;
  char    _line_buffer[CT_COINFILE_LINE_WIDTH_MAX];

  _path_safe = strndup(_path, CT_PATH_LENGTH_MAX);

  assert(_path_safe);

  _file = fopen(_path_safe, "r");

  assert(_file);

  ct_portfolio_t * _portfolio = (ct_portfolio_t *) malloc(sizeof(ct_portfolio_t));

  assert(_portfolio);

  _portfolio->path               = strndup(_path_safe, CT_PATH_LENGTH_MAX);
  _portfolio->head               = NULL;
  _portfolio->tail               = NULL;
  _portfolio->holdings_total_usd = 0.0;
  _portfolio->position_count     = 0;

  free(_path_safe);

  while(fgets(_line_buffer, CT_COINFILE_LINE_WIDTH_MAX, _file))
  {
    if(isupper(_line_buffer[0]))
    {
      ct_position_t * _position = ct_position_new(_line_buffer);

      if(NULL == _portfolio->head)
      {
        _portfolio->head = _position;
        _portfolio->tail = _portfolio->head;
      }
      else
      {
        _position->next        = _portfolio->tail->next;
        _portfolio->tail->next = _position;
        _portfolio->tail       = _position;
      }

      _portfolio->position_count++;
    }
  }

  fclose(_file);

  return _portfolio;
}

void
ct_portfolio_exec(ct_portfolio_t * _self)
{
  assert(_self);

  ct_tab_initscr(_self->position_count);
  ct_portfolio_position_each(_self, ct_tab_position_print);
  ct_tab_portfolio_holdings_print(_self);
  ct_tab_refresh();
  ct_tab_wait(_self);
  ct_tab_endwin();
}

void
ct_portfolio_destroy(ct_portfolio_t ** _self_p)
{
  assert(_self_p);

  if(*_self_p)
  {
    ct_portfolio_t * _portfolio = *_self_p;
    ct_position_t *  _p         = _portfolio->head;
    ct_position_t *  _tmp;

    if(_portfolio->path) free(_portfolio->path);

    while(_p)
    {
      _tmp = _p;

      ct_position_destroy(&_tmp);

      _p = _p->next;
    }

    free(_portfolio);

    *_self_p = NULL;
  }
}

