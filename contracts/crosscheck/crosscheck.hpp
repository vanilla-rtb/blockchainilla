
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include<eosiolib/singleton.hpp>
#include "checkpoint.hpp"
#include "partner_status.hpp"
#include "../common/utils.hpp"

namespace blockchainilla {
   using namespace eosio;


   class crosscheck : public contract  {

   public:
         crosscheck( account_name owner): contract{owner},
                                          partnerstate{_self,_self},
                                          partners{_self,_self}
         {}

         void addcheckp   (const checkpoint);
         void updatecheckp(const checkpoint);
         void removecheckp(const checkpoint);
         void validate    (uint64_t time_from , uint64_t time_to);
         void disengage   (account_name partner);

   private:
        ///@abi table partners i64
        struct partner {
            checkpoint  check;
            account_name primary_key() const { return check.time_begin; }
        };
       using partner_table_t = eosio::multi_index<N(partnertable), partner>;

        //internal structure not exported for now via abi
        struct partner_state {
            account_name partner{};
            PartnerStatus state{PartnerStatus::Undefined};
        };

        eosio::singleton<N(partnerstate), partner_state> partnerstate;
        partner_table_t partners;
   };

}
