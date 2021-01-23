#ifndef CT_PORTFOLIO_H
#define CT_PORTFOLIO_H

#include "ct_pathtools.h"
#include "ct_position.h"

typedef enum ct_portfolio_error_t
{
  CT_PORTFOLIO_ERROR_NONE = 1 << 0
} ct_portfolio_error_t;

typedef struct ct_portfolio_s
{
  char *          path;
  ct_position_t * head;
  ct_position_t * tail;
  int             position_count;
  double          holdings_total_usd;

  ct_portfolio_error_t error;
} ct_portfolio_t;

typedef void (* ct_position_iterator)(ct_position_t * _self);

ct_portfolio_error_t
ct_portfolio_error_get(ct_portfolio_t * const _self);

void
ct_portfolio_position_each(ct_portfolio_t * _self, ct_position_iterator _f);

ct_portfolio_t *
ct_portfolio_new(const char * const _path);

void
ct_portfolio_exec(ct_portfolio_t * _self);

void
ct_portfolio_destroy(ct_portfolio_t ** _self_p);

#endif // CT_PORTFOLIO_H

