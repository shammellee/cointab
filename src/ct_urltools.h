#ifndef URLTOOLS_H
#define URLTOOLS_H

#include <stdlib.h>

#define CT_URL_LENGTH_MAX 2047
#define CT_USER_AGENT_LINUX_CHROME "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36"
#define ct_urltools_set_api_url(buffer, symbol) ;snprintf(buffer, CT_URL_LENGTH_MAX, "https://api.pro.coinbase.com/products/%s/ticker", symbol);

#endif // URLTOOLS_H

