#include "crosscheck.hpp"
#include <string>

namespace blockchainilla {
   using namespace eosio;

    //@abi action
    void crosscheck::addcheckp(const checkpoint check) {
        require_auth(_self);
        ///TODO: add asserts for same partner
        partners.emplace(_self, [&](auto && p) { //payer = _self
            p.check = check;
        });
    }

    //@abi action
    void crosscheck::updatecheckp(const checkpoint check) {
        require_auth(_self);
        auto itr = partners.find(check.partner) ;
        ///TODO: add asserts
        partners.modify(itr, _self, [&](auto && p){
            p.check = check;
        });
    }

    //@abi action
    void crosscheck::removecheckp(const checkpoint check) {
        require_auth(_self);
        auto itr = partners.find(check.partner) ;
        ///TODO: add asserts
        partners.erase(itr);
    }
         
}

EOSIO_ABI( blockchainilla::crosscheck, (addcheckp)(updatecheckp)(removecheckp) )

