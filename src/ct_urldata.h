#ifndef CT_URLDATA_H
#define CT_URLDATA_H

#include <stdlib.h>

typedef struct ct_urldata_s
{
  char * memory;
  size_t size;
  long   http_code;
} ct_urldata_t;

ct_urldata_t *
ct_urldata_new(const char * const symbol);

void
ct_urldata_destroy(ct_urldata_t ** urldata);
#endif // CT_URLDATA_H

