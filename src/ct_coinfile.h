#ifndef CT_COINFILE_H
#define CT_COINFILE_H

#include <stdlib.h>

#define CT_COINFILE_LINE_WIDTH_MAX   256
#define CT_COINFILE_TOKEN_DELIMITERS " \n"

enum ct_coinfile_field_t
{
  // Values represent the token position in the coinfile
  CT_FILE_FIELD_SYMBOL         = 0
  ,CT_FILE_FIELD_QUANTITY      = 1
  ,CT_FILE_FIELD_BUY_PRICE_USD = 2
};

#endif // CT_COINFILE_H

