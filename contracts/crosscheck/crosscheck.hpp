
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/time.hpp>
#include "checkpoint.hpp"
#include "partner_status.hpp"
#include "../common/utils.hpp"

namespace blockchainilla {
   using namespace eosio;


   class crosscheck : public contract  {

   public:
         crosscheck( account_name owner): contract{owner}
         {}

         void addcheckp   (const checkpoint);
         void removecheckp(const checkpoint);
         void validate    (account_name partner, eosio::time_point  time_from , eosio::time_point  time_to);
         void engage      (account_name partner);
         void disengage   (account_name partner);
         void dispute     (account_name partner);
         void rmpartner   (account_name partner);


   private:
        ///@abi table partnertable i64
        struct partner {
            checkpoint  check;
            uint64_t primary_key() const { return check.time_begin.elapsed.count(); }
        };
       using partner_table_t = eosio::multi_index<N(partnertable), partner>;

        //internal structure not exported for now via abi
        struct partner_state {
            account_name partner{};
            eosio::time_point timestamp;
            PartnerStatus state{PartnerStatus::Undefined};
            partner_state& operator=(const PartnerStatus &s) {
                state = s;
                timestamp = eosio::time_point(microseconds(current_time()));
                return *this;
            }
        };

        using partner_state_t = eosio::singleton<N(partnerstate), partner_state>;

        template<typename PartnerStats>
        auto validate_get_partner(PartnerStats && stats, account_name partner){
            auto ps = stats.get_or_default();
            eosio_assert( ps.partner == partner,
                          (std::string("partner [")+name{partner}.to_string()+"] is not valid for this conrtact").c_str() );
            eosio_assert( ps.state == PartnerStatus::Active, (std::string("partner is not Active {")+std::to_string((int)ps.state)+"}").c_str() );
            return ps;
        }
   };

}
