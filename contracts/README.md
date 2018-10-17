# crosscheck-contract
nodeos -e -p eosio --max-transaction-time 50 --plugin eosio::chain_api_plugin --plugin eosio::history_api_plugin &> /tmp/nodeos.out &

```bash
cleos create account eosio sspcrosschek EOS6yA2bVer75LLhPA6XMKRVDUsfojfh8pvhAcAiK959nRgG9WFTq
cleos create account eosio dspcrosschek EOS6yA2bVer75LLhPA6XMKRVDUsfojfh8pvhAcAiK959nRgG9WFTq
eosiocpp -o contracts/crosscheck/crosscheck.wast contracts/crosscheck/crosscheck.cpp
eosiocpp -g contracts/crosscheck/crosscheck.abi contracts/crosscheck/crosscheck.cpp
cleos set contract sspcrosschek contracts/crosscheck -p crosscheck1@active
cleos set contract dspcrosschek contracts/crosscheck -p crosscheck1@active
```

This contracts is utilizing a view to each other , so ssp can look at dsp stats and visa versa those are not 
yet encrypted data contracts and only server as POC for now


