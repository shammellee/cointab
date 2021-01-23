#ifndef CT_POSITION_H
#define CT_POSITION_H

#include <stdlib.h>

#define CT_POSITION_SYMBOL_LENGTH_MAX 9

typedef enum ct_position_error_t
{
  CT_POSITION_ERROR_NONE                = 1 << 0
  ,CT_POSITION_ERROR_FIELD_OUT_OF_RANGE = 1 << 1
} ct_position_error_t;

typedef struct ct_position_s
{
  int    id;
  char * raw;
  char * symbol;
  double quantity;
  double price_usd;
  double buy_usd;
  double bid_usd;
  double ask_usd;
  double size;
  double volume;
  double yield_usd;
  char * time;

  ct_position_error_t error;

  struct ct_position_s * next;

} ct_position_t;


ct_position_error_t
ct_position_error_get(ct_position_t * const _self);

void
ct_position_update(ct_position_t * _self);

ct_position_t *
ct_position_new();

void
ct_position_destroy(ct_position_t ** _self_p);

#endif // CT_POSITION_H

