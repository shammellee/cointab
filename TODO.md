# TODO

* Handle duplicate symbol inquiries
* Add EMAs
* Replace curl with websockets

1. parse cointab file containing data to look up (newline-delimited)
1. create API request to get market data
1. display market data as table
  * green/red indicators for take
1. repeat step 2 on interval

## Cointab File

```
#symbol  quantity   buy price
BTC-USD  0.47120589 30001
CGLD-USD 239.5      2.13
```

## Cointab Output

Cointab        Last update: 2021-02-05 14:23:38

Symbol    Price     EMA12     EMA26     Yield
BTC-USD   32661.72  32021.81  32016.57  3212.44
CGLD-USD  0.18      0.11      0.18      -27.13
LTC-USD   135.76    1.33.16   1.09      42.82

Total Yield                             3228.13

