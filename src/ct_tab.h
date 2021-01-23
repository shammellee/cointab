#ifndef CT_TAB_H
#define CT_TAB_H

#include "ct_portfolio.h"

#define CT_TAB_LEFT_PADDING  1
#define CT_TAB_RIGHT_PADDING 1

void
ct_tab_initscr(const int _position_count);

void
ct_tab_clear();

void
ct_tab_position_print(ct_position_t * const _self);

void
ct_tab_portfolio_holdings_print(ct_portfolio_t * const _self);

void
ct_tab_refresh();

void
ct_tab_wait(ct_portfolio_t * const _self);

void
ct_tab_endwin();

#endif // CT_TAB_H

