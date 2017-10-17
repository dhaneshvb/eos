/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include "storage.hpp"
#include "storage.gen.hpp"
#include "bancor.hpp"
#include <eoslib/print.hpp>

using namespace eos;

namespace storage { 

   void allocate_storage(const Transfer& transfer) {
      // calculate storage to allocate for transfer
      Capacity capacity;
      CapacityTable::get(N(storage), capacity);
      uint64_t percent_space = bancor::allocate(transfer.amount);
      capacity.reserve = double_add(capacity.reserve, percent_space);
      CapacityTable::store(capacity, N(storage));

      // modify user allocation table
      Allocation allocation;
      AllocationTable::get(transfer.from, allocation, transfer.from);
      print(allocation.totalPaid);
      allocation.percentAllocated = double_add(allocation.percentAllocated, percent_space);
      allocation.totalPaid += transfer.amount;
      AllocationTable::store(allocation, transfer.from);
   }

   void deallocate_storage( const Transfer& transfer ) {
      Capacity capacity;
      CapacityTable::get(N(storage), capacity);
      Allocation allocation;
      AllocationTable::get(transfer.to, allocation, transfer.to);
      assert(transfer.amount <= allocation.totalPaid, "allocated eos to storage less than transfer amount");
      uint64_t average_price = double_div(allocation.percentAllocated, i64_to_double(allocation.totalPaid));
      uint64_t percent_freed = double_mult(i64_to_double(transfer.amount), average_price);
      capacity.reserve = double_sub(capacity.reserve, percent_freed);
      allocation.totalPaid -= transfer.amount;
      allocation.percentAllocated = double_sub(allocation.percentAllocated, percent_freed);
      CapacityTable::store(capacity, N(storage));
      AllocationTable::store(allocation, transfer.to);
   }

   void apply_storage_transfer( const Transfer& transfer ) {

      if (transfer.from == N(storage) ) {
          requireAuth( transfer.to );
          deallocate_storage( transfer );
      } else if (transfer.to == N(storage) ) {
          requireAuth( transfer.from );
          allocate_storage( transfer );
      } else {
          assert(0, "unknown transfer" );
      }
   }

   bool validate_ipfspath( String ipfspath ) {
      // To be implemented
      return true;  
   }

   bool validate_eospath( String eospath ) {
      // To be implemented
      return true;
   }

   void apply_storage_setlink(const Link& link) {
      requireAuth( link.owner );
      validate_ipfspath( link.ipfspath );
      validate_eospath( link.eospath );
      Bytes linkBytes = valueToBytes<Link>(link);
      Bytes eospathBytes = valueToBytes<String>(link.eospath);
      Capacity capacity;
      CapacityTable::get(N(storage), capacity);
      Allocation allocation;
      AllocationTable::get(link.owner, allocation, link.owner);
      uint64_t totalBytes = double_mult(i64_to_double(capacity.supply), allocation.percentAllocated);
      uint64_t availableBytes = double_sub(totalBytes, i64_to_double(allocation.usedBytes));
      
      if (link.requestHosting > 0) {
          assert(link.size <= availableBytes, "insufficient space to store file");
      }
      LinkTable::store((char*)eospathBytes.data, eospathBytes.len, (char*)linkBytes.data, linkBytes.len);
   }
   
   void apply_storage_removelink( const RemoveLink& removelink ) {
      requireAuth( removelink.owner );
      validate_eospath( removelink.eospath ); 
      Bytes eospathBytes = valueToBytes<String>(removelink.eospath);
      
      char* buffer = (char*)eos::malloc(4098);
      auto len = LinkTable::load((char*)eospathBytes.data, eospathBytes.len, buffer, 4098, removelink.owner);
      assert(len > 0, "file not found");
      Bytes bytes;
      bytes.len = len;
      bytes.data = (uint8_t*)buffer;
      Link link = bytesToValue<Link>(bytes);
      
      if (!removelink.keep_link) {
         auto len = LinkTable::remove((char*)eospathBytes.data, eospathBytes.len, removelink.owner);
         assert(len > 0, "eos file path not found");
      } else {
         link.accepted = 0;
         LinkTable::update((char*)eospathBytes.data, eospathBytes.len, buffer, len, removelink.owner);
      }
     
      if (link.requestHosting > 0) { 
      	  Allocation allocation;
          AllocationTable::get(removelink.owner, allocation, removelink.owner);
          allocation.usedBytes -= link.size;
          AllocationTable::store(allocation, removelink.owner);
      }
      eos::free(buffer);
   }
   
   void apply_storage_acceptstore( const Store& store ) {
   }
   
   void apply_storage_rejectstore( const Reject& reject ) {
   }
}  // namespace storage

using namespace storage;

extern "C" {
    void init()  {
       Capacity capacity { N(storage), 1024ll*1024ll*1024ll*1024ll*1024ll, 0 };
       CapacityTable::store(capacity, N(storage));
    }

    /// The apply method implements the dispatch of events to this contract
    void apply( uint64_t code, uint64_t action ) {
       if( code == N(eos) ) {
          if( action == N(transfer) ) {
              storage::apply_storage_transfer(eos::currentMessage<Transfer>());
          } 
          else {
               assert(0, "unknown message");
          } 
       } else if ( code == N(storage) ) {
          if (action == N(setlink) ) {
               storage::apply_storage_setlink(eos::currentMessage<Link>());
          } else if (action == N(removelink) ) {
               storage::apply_storage_removelink(eos::currentMessage<RemoveLink>());
          } else if (action == N(acceptstore) ) {
               storage::apply_storage_acceptstore(eos::currentMessage<Store>());
          } else if (action == N(rejectstore) ) {
               storage::apply_storage_rejectstore(eos::currentMessage<Reject>());
          } else {
               assert(0, "unknown message");
          }
       } else {
           assert(0, "unknown code");
       }
    }
}
