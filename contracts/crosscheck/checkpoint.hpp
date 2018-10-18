#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

namespace blockchainilla {

    struct checkpoint {
        //time begin  since epoch, use lower_bound = t1 and upper_bound = t2 to retrieve recrods
        uint64_t time_begin{};
        account_name partner{};
        uint64_t views{};
        eosio::asset amount{};

        EOSLIB_SERIALIZE(checkpoint, (time_begin)(partner)(views)(amount))
    };

}

