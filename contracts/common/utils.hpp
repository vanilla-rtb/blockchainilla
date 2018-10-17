#pragma once
/**
 *  @file
 *  @copyright defined in eosio/LICENSE.txt
 */

#include <eosiolib/types.hpp>

namespace eosio {

    template<typename CharT>
    static std::string to_hex( const CharT* d, uint32_t s ) ;
 
    static key256 checksum160_to_key256(const checksum160& value) {
      key256 k;
      memset(k.data(),0,32);
      memcpy(k.data(),&value,20);
      print((std::string("DEBUG=") + to_hex((char*)k.data(),32)).c_str());  
      return k;
    }

    static key256 checksum256_to_key256(const checksum256& value) {
        const uint128_t *p128 = reinterpret_cast<const uint128_t *>(&value);
        key256 k;
        k.data()[0] = p128[0];
        k.data()[1] = p128[1];
        return k;
    }

    uint8_t from_hex( char c ) {
        if( c >= '0' && c <= '9' )
            return c - '0';
        if( c >= 'a' && c <= 'f' )
            return c - 'a' + 10;
        if( c >= 'A' && c <= 'F' )
            return c - 'A' + 10;
        eosio_assert( false, (std::string(&c,1) + "Invalid hex character").c_str() );
        return 0;
    }

    template<typename CharT>
    static std::string to_hex( const CharT* d, uint32_t s ) {
        std::string r;
        const char* to_hex="0123456789abcdef";
        uint8_t* c = (uint8_t*)d;
        for( uint32_t i = 0; i < s; ++i ) {
            (r += to_hex[(c[i] >> 4)]) += to_hex[(c[i] & 0x0f)];
        }
        return r;
    }

    template<typename String, typename CharT>
    size_t from_hex( const String& hex_str, CharT* out_data, size_t out_data_len ) {
        typename String::const_iterator i = hex_str.begin();
        uint8_t* out_pos = (uint8_t*)out_data;
        uint8_t* out_end = out_pos + out_data_len;
        while( i != hex_str.end() && out_end != out_pos ) {
            *out_pos = from_hex( *i ) << 4;
            ++i;
            if( i != hex_str.end() )  {
                *out_pos |= from_hex( *i );
                ++i;
            }
            ++out_pos;
        }
        return out_pos - (uint8_t*)out_data;
    }

    template<typename Iter, typename Idx>
    void erase(Idx&& idx,  Iter first, Iter last) {
        for(auto itr=first; itr!=last;) {
            idx.erase(itr);
        }
    }

    template<typename Iter, typename Idx, typename Funct>
    void erase(Idx&& idx,  Iter first, Iter last, Funct func) {
        for(auto itr=first; itr!=last;) {
            func(itr);
            itr = idx.erase(itr);
        }
    }
} //namespace
