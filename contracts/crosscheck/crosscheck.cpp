#include "crosscheck.hpp"
#include <chrono>
#include <numeric>

namespace blockchainilla {
   using namespace eosio;

    //@abi action
    void crosscheck::addcheckp(const checkpoint check) {
        ///TODO: time should be always 01:00 , 02:00 , 15:00 , 16:00 within bounderies
        /// use std::chrono to truncate incoming time seconds since epoch into 1:00 , 2:00 , 15:00 , 16:00
        /// if truncated then maybe addcheckp + updatecheckp => updatecheckp ????
        ///https://stackoverflow.com/questions/15957805/extract-year-month-day-etc-from-stdchronotime-point-in-c
        require_auth(_self);
        auto itr = partners.find(check.time_begin);
        auto && time_begin_str = std::to_string(check.time_begin);
        eosio_assert( itr == partners.end(), (std::string("time slot already exist [")+time_begin_str+"]").c_str() );
        partners.emplace(_self, [&](auto && p) { //payer = _self
            p.check = check;
        });
    }

    //@abi action
    void crosscheck::updatecheckp(const checkpoint check) {
        ///TODO: time should be always 01:00 , 02:00 , 15:00 , 16:00 within bounderies
        /// use std::chrono to truncate incoming time seconds since epoch into 1:00 , 2:00 , 15:00 , 16:00
        require_auth(_self);
        auto itr = partners.find(check.time_begin) ;
        eosio_assert( itr->check.partner == check.partner, (std::string("partner ")+name{itr->check.partner}.to_string()+"!="+name{check.partner}.to_string()).c_str() );
        partners.modify(itr, _self, [&](auto && p){
            p.check = check;
        });
    }

    //@abi action
    void crosscheck::removecheckp(const checkpoint check) {
        require_auth(_self);
        auto itr = partners.find(check.time_begin) ;
        ///TODO: add assertions
        partners.erase(itr);
    }

    //@abi action
    void crosscheck::validate(uint64_t time_from , uint64_t time_to) { //not sure if we need input parameter or should iterate for all ?
        require_auth(_self);
        auto && p = partnerstate.get_or_default(); ///TODO: assert
        ///TODO: implement this action by checking thresh hold say > 10% diffs between your stats and the partners
        partner_table_t other{p.partner, p.partner};
        //auto to_itr   = other.upper_bound((std::chrono::seconds(check.time_begin) + std::chrono::hours(1)).count());
        auto count_views = [](auto && init , auto && p) { return init + p.check.views; };
        auto myviews = std::accumulate(partners.lower_bound(time_from), partners.lower_bound(time_to), 0, count_views);
        auto partner_views = std::accumulate(other.lower_bound(time_from), other.upper_bound(time_to), 0, count_views);
        //if ( myviews > partner_views*0.1 || myviews < partner_views/1.1)
        // send action disengage to this contract
    }

    //@abi action
    void crosscheck::disengage(account_name partner)  { //Maybe we don't need partner here this is single partner contract ?
        require_auth(_self);
        auto && p = partnerstate.get_or_default();
        eosio_assert( p.partner == partner, (std::string("partner [")+name{partner}.to_string()+"] is not valid for this conrtact").c_str() );
        eosio_assert( p.state == PartnerStatus::Active, (std::string("partner is not Active {")+std::to_string((int)p.state)+"}").c_str() );
        p.state = PartnerStatus::OnHold;
    }


}

EOSIO_ABI( blockchainilla::crosscheck, (addcheckp)(updatecheckp)(removecheckp)(validate)(disengage) )

