#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

namespace blockchainilla {

    struct checkpoint {
        account_name partner{};
        uint64_t CPM{};
        uint64_t CPC{};
        uint64_t CPA{};
        uint64_t amount{};
        checksum256 user_uuid{}; //hash256 of a user uuid from ssp
        uint64_t check_period{}; //timelaps to trigger metrics checks

        EOSLIB_SERIALIZE(checkpoint, (partner)(CPM)(CPC)(CPA)(amount)(user_uuid))
    };

}

