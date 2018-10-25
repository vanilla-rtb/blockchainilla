#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>

namespace blockchainilla {

    struct checkpoint {
        //time begin  since epoch, use lower_bound = t1 and upper_bound = t2 to retrieve recrods
        eosio::time_point time_begin{};
        account_name partner{};
        uint64_t views{};
        eosio::asset amount{};

        EOSLIB_SERIALIZE(checkpoint, (time_begin)(partner)(views)(amount))
    };

}

