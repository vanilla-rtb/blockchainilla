# crosscheck-contract
nodeos -e -p eosio --plugin eosio::chain_api_plugin --plugin eosio::history_api_plugin &> /tmp/nodeos.out &

```bash
cleos create account eosio sspcrosschek EOS6yA2bVer75LLhPA6XMKRVDUsfojfh8pvhAcAiK959nRgG9WFTq
cleos create account eosio dspcrosschek EOS6yA2bVer75LLhPA6XMKRVDUsfojfh8pvhAcAiK959nRgG9WFTq
eosiocpp -o contracts/crosscheck/crosscheck.wast contracts/crosscheck/crosscheck.cpp
eosiocpp -g contracts/crosscheck/crosscheck.abi contracts/crosscheck/crosscheck.cpp
cleos set contract sspcrosschek contracts/crosscheck -p sspcrosschek@active
cleos set contract dspcrosschek contracts/crosscheck -p dspcrosschek@active
```

This contracts is utilizing a view to each other , so ssp can look at dsp stats and visa versa those are not 
yet encrypted data contracts and only server as POC for now

### DSP adds SSP partner 
```cleos push action dspcrosschek engage '["sspcrosschek"]' -p dspcrosschek```

### SSP adds DSP partner 
```cleos push action sspcrosschek engage '["dspcrosschek"]' -p sspcrosschek```

### DSP adds views stats 
```cleos push action dspcrosschek addcheckp '{"":{"time_begin":"2018-10-26T04:56:36.003" ,"partner":"sspcrosschek","views":7777777777,"amount":"5.0000 SYS"}}' -p dspcrosschek

cleos push action dspcrosschek addcheckp '{"":{"time_begin":"2018-10-26T04:59:48.033" ,"partner":"sspcrosschek","views":7777777778,"amount":"5.0000 SYS"}}' -p dspcrosschek

cleos push action dspcrosschek addcheckp '{"":{"time_begin":"2018-10-26T04:56:36.003" ,"partner":"sspcrosschek","views":7777777779,"amount":"5.0000 SYS"}}' -p dspcrosschek

cleos push action dspcrosschek addcheckp '{"":{"time_begin":"2018-10-26T06:50:35.123" ,"partner":"sspcrosschek","views":8888888888,"amount":"5.0000 SYS"}}' -p dspcrosschek
```

### check for DSP stats
```
cleos get table dspcrosschek sspcrosschek  partnertable
{
  "rows": [{
    "check": {
        "time_begin": "2018-10-26T04:00:00.000",
        "partner": "sspcrosschek",
        "views": "7777777778",
        "amount": "5.0000 SYS"
      }
    },{
      "check": {
        "time_begin": "2018-10-26T05:00:00.000",
        "partner": "sspcrosschek",
        "views": "7777777779",
        "amount": "5.0000 SYS"
      }
    },{
      "check": {
        "time_begin": "2018-10-26T06:00:00.000",
        "partner": "sspcrosschek",
        "views": "888888888888",
        "amount": "15.0000 SYS"
      }
    }
  ],
  "more": false
}
```

### SSP adds views stats
```
cleos push action sspcrosschek addcheckp '{"":{"time_begin":"2018-10-26T04:56:36.003" ,"partner":"dspcrosschek","views":7777555577,"amount":"15.0000 SYS"}}' -p sspcrosschek
```

### check for SSP stats
```
cleos get table sspcrosschek dspcrosschek  partnertable
{
  "rows": [{
      "check": {
        "time_begin": "2018-10-26T04:00:00.000",
        "partner": "dspcrosschek",
        "views": "7777555577",
        "amount": "15.0000 SYS"
      }
    }
  ],
  "more": false
}
```

### DSP to validate against SSP stats will trigger disengage action that will put SSP partner OnHold as decrepency exceeds 10% (TODO: configurable) - DSP can also use this contract state and stop bidding on the impressions until descepency resolved.

```
cleos push action dspcrosschek validate '["sspcrosschek", "2018-10-26T04:00:00.000" , "2018-10-26T05:00:00.000"]' -p dspcrosschek
executed transaction: d190489a88a190dbc5220b9ba0292fe33a3ca108f05c40e0cb7c83fbcac809a0  120 bytes  5740 us
#  dspcrosschek <= dspcrosschek::validate       {"partner":"sspcrosschek","time_from":"2018-10-26T04:00:00.000","time_to":"2018-10-26T05:00:00.000"}
#  dspcrosschek <= dspcrosschek::disengage      {"partner":"sspcrosschek"}
```

### DSP cannot add stats for suspended SSP ( in progress )

