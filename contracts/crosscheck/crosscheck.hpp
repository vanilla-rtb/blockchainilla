
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include<eosiolib/singleton.hpp>
#include "checkpoint.hpp"
#include "../common/utils.hpp"

namespace blockchainilla {
   using namespace eosio;

    enum class WorkFlow : int8_t {
        Open,
        Closed,
        Payout,
        Undefined
    };

   class crosscheck : public contract  {

   public:
         crosscheck( account_name owner): contract{owner},
                                          workflow{_self,_self},
                                          partners{_self,_self}
         {}

         void addcheckp(const checkpoint);
         void updatecheckp(const checkpoint);
         void removecheckp(const checkpoint);
   private:

        ///@abi table partners i64
        struct partner {
            checkpoint  check;
            account_name primary_key() const { return check.partner; }
        };
       using partner_table_t = eosio::multi_index<N(partnertable), partner>;

        //internal structure not exported for now via abi
        struct crosscheck_state {
            account_name partner{};
            WorkFlow state{WorkFlow::Undefined};
        };

        eosio::singleton<N(partnerstate), crosscheck_state> workflow;
        partner_table_t partners;
   };

}
