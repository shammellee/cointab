#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <ncurses.h>

#include "ct_portfolio.h"
#include "ct_position.h"
#include "ct_tab.h"

enum {
  CT_TAB_COLOR_PAIR_HEADER          = 1
  ,CT_TAB_COLOR_PAIR_FOOTER         = 2
  ,CT_TAB_COLOR_PAIR_YIELD_POSITIVE = 3
  ,CT_TAB_COLOR_PAIR_YIELD_NEGATIVE = 4
};

static WINDOW * _header;
static WINDOW * _table_header;
static WINDOW * _table;
static WINDOW * _footer;
static int      _y_max;
static int      _x_max;

void
ct_tab_initscr(const int _position_count)
{
  (void) _position_count;
  int _header_y             = 0
      ,_header_x            = 0
      ,_header_height       = 1
      ,_table_header_y      = _header_y + _header_height
      ,_table_header_x      = _header_x
      ,_table_header_height = 1
      ,_table_height
      ,_table_y             = _table_header_y + _table_header_height
      ,_table_x             = _header_x
      ,_footer_height       = 2
      ,_footer_y
      ,_footer_x            = _header_x
      ;

  initscr();
  curs_set(0);
  noecho();

  if(!has_colors())
  {
    printw("No colors");
    getch();
    exit(EXIT_FAILURE);
  }

  start_color();
  init_pair(CT_TAB_COLOR_PAIR_HEADER, COLOR_BLACK, COLOR_YELLOW);
  init_pair(CT_TAB_COLOR_PAIR_YIELD_POSITIVE, COLOR_GREEN, COLOR_BLACK);
  init_pair(CT_TAB_COLOR_PAIR_YIELD_NEGATIVE, COLOR_RED, COLOR_BLACK);
  init_pair(CT_TAB_COLOR_PAIR_FOOTER, COLOR_WHITE, COLOR_BLACK);

  getmaxyx(stdscr, _y_max, _x_max);

  _table_height  = _y_max;
  _table_height -= _header_height;
  _table_height -= _table_header_height;
  _table_height -= _footer_height;
  _footer_y      = _y_max - _footer_height;

  _header       = newwin(_header_height, _x_max, _header_y, _header_x);
  _table_header = newwin(_table_header_height, _x_max, _table_header_y, _table_header_x);
  _table        = newwin(_table_height, _x_max, _table_y, _table_x);
  _footer       = newwin(_footer_height, _x_max, _footer_y, _footer_x);

  refresh();

  nodelay(_table, TRUE);
  wbkgd(_header, COLOR_PAIR(CT_TAB_COLOR_PAIR_HEADER));
  mvwprintw(_header, 0, CT_TAB_LEFT_PADDING, "Cointab");

  mvwprintw(
    _table_header
    ,0
    ,CT_TAB_LEFT_PADDING
    ,"%-*s %-15s %-15s"
    ,CT_POSITION_SYMBOL_LENGTH_MAX
    ,"Symbol"
    ,"Price"
    ,"Yield"
  );

  wmove(_footer, 1, 0);
  wattron(_footer, A_REVERSE);
  wprintw(
    _footer
    ,"%-*s%-*s"
    ,CT_TAB_LEFT_PADDING
    ,""
    ,_x_max
    ,"(press q or esc to exit)"
  );
  wattroff(_footer, A_REVERSE);

  wrefresh(_header);
  wrefresh(_table_header);
  wrefresh(_footer);
}

void
ct_tab_clear()
{
}

void
ct_tab_position_print(ct_position_t * const _self)
{
  assert(_self);

  int _cp;

  wmove(_table, _self->id, CT_TAB_LEFT_PADDING);
  wclrtoeol(_table);

  wprintw(
    _table
    ,"%-*s %-15.2lf"
    ,CT_POSITION_SYMBOL_LENGTH_MAX
    ,_self->symbol
    ,_self->price_usd
  );

  if(0 == _self->yield_usd)
  {
    wprintw(_table, " %-15s", "--.--");
  }
  else
  {
    if(0 < _self->yield_usd)
    {
      _cp = CT_TAB_COLOR_PAIR_YIELD_POSITIVE;
    }
    else
    {
      _cp = CT_TAB_COLOR_PAIR_YIELD_NEGATIVE;
    }

    wattron(_table, COLOR_PAIR(_cp));
    wprintw(_table, " %-15.2lf", _self->yield_usd);
    wattroff(_table, COLOR_PAIR(_cp));
  }

  wclrtoeol(_table);

  ct_tab_refresh();
}

void
ct_tab_portfolio_holdings_print(ct_portfolio_t * const _self)
{
  assert(_self);

  int    _cp;
  double _h = _self->holdings_total_usd;

  wclrtoeol(_footer);

  mvwprintw(_footer, 0, _x_max - CT_TAB_RIGHT_PADDING - 12 - 15, "%s", "Total Yield:");

  if(0.0 == _h)
  {
    wprintw(_footer, "%15s", "--.--");
  }
  else
  {
    if(0.0 < _h) _cp = CT_TAB_COLOR_PAIR_YIELD_POSITIVE;
    else         _cp = CT_TAB_COLOR_PAIR_YIELD_NEGATIVE;

    wattron(_footer, COLOR_PAIR(_cp));
    wprintw(_footer, "%15.2lf", _h);
    wattroff(_footer, COLOR_PAIR(_cp));
  }

  wrefresh(_footer);
}

void
ct_tab_refresh()
{
  wrefresh(_table);
  wrefresh(_footer);
}

void
ct_tab_wait(ct_portfolio_t * const _self)
{
  char c;

  keypad(_table, true);

  while(1)
  {
    c = wgetch(_table);

    if(
      'q' == c
      || 27 == c
    )
    {
      break;
    }

    ct_portfolio_position_each(_self, ct_position_update);
    ct_portfolio_position_each(_self, ct_tab_position_print);
    ct_tab_portfolio_holdings_print(_self);
  }
}

void
ct_tab_endwin()
{
  endwin();
}

