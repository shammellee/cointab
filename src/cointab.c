#include "cointab.h"

int main(void)
{
  ct_portfolio_t * portfolio = ct_portfolio_new("/Users/shammel/.coinrc");

  ct_portfolio_exec(portfolio);

  ct_portfolio_destroy(&portfolio);

  return EXIT_SUCCESS;
}

