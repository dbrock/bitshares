#pragma once

#include <bts/blockchain/asset.hpp>
#include <bts/blockchain/types.hpp>

namespace bts { namespace blockchain {

   struct market_index_key
   {
      market_index_key( const price& price_arg = price(), 
                        const address& owner_arg = address() )
      :order_price(price_arg),owner(owner_arg){}

      price   order_price;
      address owner;

      friend bool operator == ( const market_index_key& a, const market_index_key& b )
      {
         return a.order_price == b.order_price && a.owner == b.owner;
      }
      friend bool operator < ( const market_index_key& a, const market_index_key& b )
      {
         if( a.order_price < b.order_price ) return true;
         if( a.order_price > b.order_price ) return false;
         return  a.owner < b.owner;
      }
   };


   struct order_record 
   {
      order_record():balance(0){}
      order_record( share_type b )
      :balance(b){}

      bool is_null() const { return 0 == balance; }

      share_type       balance;
   };
   typedef fc::optional<order_record> oorder_record;

   enum order_type_enum
   {
      bid_order,
      ask_order,
      short_order,
      cover_order
   };

   struct market_order 
   {
      market_order( order_type_enum t, market_index_key k, order_record s )
      :type(t),market_index(k),state(s){}
      market_order( order_type_enum t, market_index_key k, order_record s, share_type c )
      :type(t),market_index(k),state(s),collateral(c){}
       
       
      market_order(){}

      string            get_id()const;
      asset             get_balance()const; // funds available for this order
      price             get_price()const;
      asset             get_quantity()const;
      asset             get_quote_quantity()const;
      address           get_owner()const { return market_index.owner; }

      order_type_enum       type;
      market_index_key      market_index;
      order_record          state;
      optional<share_type>  collateral; 
   };


   struct market_transaction
   {
      market_transaction(){}

      address  bid_owner;
      address  ask_owner;
      price    bid_price;
      price    ask_price;
      asset    bid_paid;
      asset    bid_received;
      asset    ask_paid;
      asset    ask_received;
      asset    fees_collected;
   };

   typedef optional<market_order> omarket_order;

   struct collateral_record
   {
      collateral_record():collateral_balance(0),payoff_balance(0){}
      bool is_null() const { return 0 == payoff_balance; }

      share_type      collateral_balance;
      share_type      payoff_balance;
   };
   typedef fc::optional<collateral_record> ocollateral_record;

} } // bts::blockchain

FC_REFLECT_ENUM( bts::blockchain::order_type_enum, (bid_order)(ask_order)(short_order)(cover_order) )
FC_REFLECT( bts::blockchain::market_index_key, (order_price)(owner) )
FC_REFLECT( bts::blockchain::order_record, (balance) )
FC_REFLECT( bts::blockchain::collateral_record, (collateral_balance)(payoff_balance) )
FC_REFLECT( bts::blockchain::market_order, (type)(market_index)(state)(collateral) )
FC_REFLECT_TYPENAME( std::vector<bts::blockchain::market_transaction> )
FC_REFLECT( bts::blockchain::market_transaction, 
            (bid_owner)
            (ask_owner)
            (bid_price)
            (ask_price)
            (bid_paid)
            (bid_received)
            (ask_paid)
            (ask_received)
            (fees_collected) 
          )
