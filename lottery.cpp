#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include "oraclize/eos_api.hpp"



using namespace eosio;

class [[eosio::contract]] lottery : public eosio::contract {
  public:
      using contract::contract;

    [[eosio::action]]
    void getrand(){

        require_auth(_self);
        
        print("Requesting Data from Oraclize");
        oraclize_query("URL", "json(https://www.random.org/integers/?num=1&min=1&max=100&col=1&base=10&format=plain&rnd=new)");  
        
    }

    [[eosio::action]]
    void callback(capi_checksum256 queryId, std::vector<uint8_t> result, std::vector<uint8_t> proof)
    {
        require_auth(oraclize_cbAddress());

        const std::string result_str = vector_to_string(result);
        afterquery(result_str);
    }

    [[eosio::action]]
    void register(name user,uint64_t number,uint64_t lotteryid){

        require_auth(_self);

        recordindex records(_code, _code.value);
        auto iterator = records.find(user.value);
        if( iterator == records.end() )
        {
            records.emplace(user, [&]( auto& row ) {
                row.key = user;
                row.number = number;
                row.lotteryid = lotteryid;
            });
        }
    }

    void afterquery(result_str,uint64_t lotid){

        std::int winner = std::atoi(result_str.c_str());

        lotindex tests(_self, get_first_receiver().value); 

        tests.emplace(_self, [&]( auto& row ) {

            row.lotid = lotid;
            row.number = number;
        });

        rem(lotid);
    }

    void rem(uint64_t lotteryid){

        recordindex tests(_self, get_first_receiver().value);

        auto acc_index = tests.get_index<name("lot")>();
        auto itr_lower = acc_index.lower_bound(lotteryid);
        auto itr_upper = acc_index.upper_bound(lotteryid);
         

        while(itr_lower != itr_upper){

            if(itr_lower->lotteryid == lotteryid){

                auto toDel = itr_lower;

                itr_lower++;

                auto iterator = tests.find(toDel->id);
                tests.erase(iterator);
            }
            else{

                itr_lower++;
            }
        } 
    }

    struct [[eosio::table]] record{

      name user;
      uint64_t number;
      uint64_t lotteryid;

      uint64_t primary_key() const { return user.value; }
      uint64_t get_lotid() const {return lotteryid;}
    };

    struct [[eosio::table]] lotwin{

      uint64_t lotid;
      uint64_t number;

      uint64_t primary_key() const { return id; }
    };

    typedef eosio::multi_index<"lotwin"_n,lotwin> lotwinindex;
    typedef eosio::multi_index<"record"_n, record, indexed_by<"lot"_n, const_mem_fun<record, uint64_t, &record::get_lotid>>> recordindex;
}
