#include "crosscheck.hpp"
#include <numeric>

namespace eosio {
    //missing implementation in <eosiolib/time.hpp>
    time_point::operator std::string() const {
        return std::to_string(elapsed.count());
    }
}

namespace blockchainilla {
   using namespace eosio;

    //@abi action
    void crosscheck::addcheckp(const checkpoint check) {
        ///time should be always 01:00 , 02:00 , 15:00 , 16:00 within bounderies to save on RAM
        ///https://stackoverflow.com/questions/15957805/extract-year-month-day-etc-from-stdchronotime-point-in-c
        require_auth(_self);
        partner_table_t partnerviews{_self,check.partner};
        auto && hours = eosio::time_point(eosio::hours(check.time_begin.time_since_epoch().to_seconds()/3600));
        partner_state_t partnerstate{_self, check.partner};
        eosio_assert( partnerstate.get_or_default().partner == check.partner, "check.partnerviews is not in partnerstate");
        eosio_assert( partnerstate.get_or_default().timestamp <= check.time_begin, "check.time_begin preceeds partnerstate.timestamp");
        auto itr = partnerviews.find(hours.elapsed.count()); //hours since epoch
        if ( itr == partnerviews.end() ) {
            partnerviews.emplace(_self, [&](auto &&p) { //payer = _self
                p.check = check;
                p.check.time_begin = hours;
            });
        } else {
            partnerviews.modify(itr, _self, [&](auto && p){ //payer = _self
                p.check = check;
                p.check.time_begin = hours;
            });
        }
    }


    //@abi action
    void crosscheck::removecheckp(const checkpoint check) {
        require_auth(_self);
        partner_table_t partnerviews{_self,check.partner};
        auto && hours = eosio::time_point(eosio::hours(check.time_begin.time_since_epoch().to_seconds()/3600));
        auto itr = partnerviews.find(hours.elapsed.count()) ; //hours since epoch
        eosio_assert( itr != partnerviews.end(), (std::string("time slot does not exist [")+std::string(check.time_begin)+"]").c_str() );
        partnerviews.erase(itr);
    }

    //@abi action
    void crosscheck::validate(account_name partner, eosio::time_point time_from , eosio::time_point time_to) {
        require_auth(_self);
        partner_state_t partnerstate{_self, partner};
        auto && ps = partnerstate.get_or_default();

        eosio_assert( ps.partner == partner, (std::string("partner [")+name{partner}.to_string()+"] is not valid for this conrtact").c_str() );
        eosio_assert( ps.state == PartnerStatus::Active, (std::string("partner [")+name{partner}.to_string()+"] is not Active").c_str() );

        partner_table_t partner_views_table{ps.partner, _self};
        partner_table_t myviews_table{_self, ps.partner};

        auto count_views = [](auto && init , auto && p) { return init + p.check.views; };
        auto time_from_hours = time_from.time_since_epoch().to_seconds()/3600;
        auto time_to_hours = time_to.time_since_epoch().to_seconds()/3600;
        auto myviews = std::accumulate(myviews_table.lower_bound(time_from_hours), myviews_table.upper_bound(time_to_hours), 0, count_views);
        auto partner_views = std::accumulate(partner_views_table.lower_bound(time_from_hours), partner_views_table.upper_bound(time_to_hours), 0, count_views);

        if ( myviews > partner_views*0.1 || myviews < partner_views/1.1) {
            action(permission_level{_self, N(active)},
                   _self, N(disengage),
                   std::make_tuple(partner)
            ).send();
        }
    }

    //@abi action
    void crosscheck::disengage(account_name partner)  {
        require_auth(_self);
        partner_state_t partnerstate{_self, partner};
        auto && ps = validate_get_partner(partnerstate,partner);
        ps = PartnerStatus::OnHold;
        partnerstate.set(ps,_self);
    }

    //@abi action
    void crosscheck::engage(account_name partner)  {
        require_auth(_self);
        partner_state_t partnerstate{_self, partner};
        auto && ps = partnerstate.get_or_default();
        if ( !ps.partner ) {
            ps.partner = partner;
            ps = PartnerStatus::Active;
            partnerstate.set(ps, _self);
        } else {
            eosio_assert( ps.partner == partner,
                          (std::string("partner [")+name{partner}.to_string()+"] does not belong to partnerstate").c_str() );
            eosio_assert(ps.state != PartnerStatus::Active,
                         (std::string("partner is already Active {") + std::to_string((int) ps.state) + "}").c_str());
            ps = PartnerStatus::Active;
            partnerstate.set(ps, _self);
        }
    }

    //@abi action
    void crosscheck::dispute(account_name partner)  {
        require_auth(_self);
        partner_state_t partnerstate{_self, partner};
        auto && ps = validate_get_partner(partnerstate,partner);
        ps = PartnerStatus::Disputed;
        partnerstate.set(ps,_self);
    }

    //@abi action
    void crosscheck::rmpartner(const account_name partner) {
        require_auth(_self);

        partner_state_t partnerstate{_self, partner};
        auto && ps = partnerstate.get_or_default();
        eosio_assert( ps.partner == partner,
                      (std::string("partner [")+name{partner}.to_string()+"] is not valid for this partnerstate").c_str() );

        partnerstate.remove();

        partner_table_t partnerviews{_self,partner};
        for ( auto && view : partnerviews ) {
            partnerviews.erase(view);
        }
    }


}

EOSIO_ABI( blockchainilla::crosscheck, (addcheckp)(removecheckp)(validate)(disengage)(engage)(rmpartner) )

