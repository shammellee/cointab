#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <curl/curl.h>

#include "ct_urldata.h"
#include "ct_urltools.h"

#define CT_API_CALLS_PER_SECONDS_MAX 3
#define CT_API_TIMEOUT_SECONDS       1

// Reference: https://everything.curl.dev/libcurl/examples/getinmem

static size_t
_write_memory_callback(
  void *  contents
  ,size_t size
  ,size_t nmemb
  ,void * userp
)
{
  size_t         _realsize = size * nmemb;
  ct_urldata_t * _data     = (ct_urldata_t *) userp;

  assert(_data);

  _data->memory = realloc(_data->memory, _data->size + _realsize + 1);

  assert(_data->memory);

  memcpy(&(_data->memory[_data->size]), contents, _realsize);

  _data->size                += _realsize;
  _data->memory[_data->size]  = 0;

  return _realsize;
}

ct_urldata_t *
ct_urldata_new(const char * const _symbol)
{
  static int _requests  = 0;
  CURL *     _curl      = curl_easy_init();
  char *     _endpoint  = (char *) malloc(sizeof(char) * CT_URL_LENGTH_MAX);
  long       _http_code = 0;
  CURLcode   _status;

  assert(_curl);
  assert(_endpoint);

  ct_urltools_set_api_url(_endpoint, _symbol);

  ct_urldata_t * _chunk = (ct_urldata_t *) malloc(sizeof(ct_urldata_t));

  assert(_chunk);

  _chunk->memory = malloc(1);  // Initialize 1 byte; grown as needed by realloc
  _chunk->size   = 0;          // No data yet

  curl_global_init(CURL_GLOBAL_ALL);

  // Specify URL
  curl_easy_setopt(_curl, CURLOPT_URL, _endpoint);

  // Send all data to write_memory_callback
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _write_memory_callback);

  // Pass "_chunk" struct to the callback function
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *) _chunk);

  // Some servers don't like requests without specifying a user-agent
  curl_easy_setopt(_curl, CURLOPT_USERAGENT, CT_USER_AGENT_LINUX_CHROME);

  // Do not verify server's SSL certificate
  curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);

  if(CT_API_CALLS_PER_SECONDS_MAX == _requests)
  {
    sleep(CT_API_TIMEOUT_SECONDS);

    _requests = 0;
  }

  _requests++;

  // Make request
  // If request is successful, _chunk.memory points to a memory block that is
  // _chunk.size bytes and contains the remote file
  _status = curl_easy_perform(_curl);

  if(_status != CURLE_OK)
  {
    fprintf(stderr, "\nRequest Error: %s\n", curl_easy_strerror(_status));

    return NULL;
  }

  curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_http_code);

  _chunk->http_code = _http_code;

  // Clean up
  free(_endpoint);
  curl_easy_cleanup(_curl);
  curl_global_cleanup();

  return _chunk;
}

void
ct_urldata_destroy(ct_urldata_t ** _self_p)
{
  assert(_self_p);

  if(*_self_p)
  {
    ct_urldata_t * _self = *_self_p;
    free(_self->memory);
    free(_self);
    *_self_p = NULL;
  }
}

