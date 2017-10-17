/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eoslib/eos.hpp>
#include <eoslib/db.hpp>

/**
 *  @defgroup examplecontract Example Storage
 *  @brief Example Contract
 *  @ingroup contractapi
 *
 */

namespace storage {
  /**
  * Represents a native token transfer
  **/
  struct PACKED( Transfer ) {
    AccountName from;
    AccountName to;
    uint64_t amount;
    String memo;  
  };

  /**
   Represents storage capacity of system; only one record
  **/
  struct PACKED( Capacity ) {
      /**
      * account name (place holder for singleton account)
      **/
      AccountName owner;

      /**
      * total available capacity in bytes
      **/ 
      uint64_t supply;

      /**
      * total reserved capacity in percent
      **/
      uint64_t reserve;
  };

  struct PACKED( Allocation ) {
      /**
      * User account name
      */
      AccountName account;
  
     /**
     * Total storage space allocated in percent
     **/
     uint64_t percentAllocated;
     
     /**
     * Total storage space used in bytes
     **/
     uint64_t usedBytes;

     /**
     * Total eos paid for storage
     **/
     uint64_t totalPaid;
  };

  /**
   Used to set a link to a file
  **/
  struct PACKED( Link ) {
      /**
      * account owner
      **/
      AccountName owner;

      /**
      * eos path
      **/
      String eospath;
      
      /**
      * ipfs file path
      **/
      String ipfspath;

      /**
      * size of file
      **/
      uint64_t size;

      /**
      * a flag that signals producers to cache file
      **/
      uint8_t requestHosting;

      /**
      * If accepted by at least one producer
      **/
      uint8_t accepted;

      /**
      * name of producer who first accepts storage
      **/
      AccountName producer;
  };

 struct PACKED( RemoveLink ) {
      /**
      * file owner
      **/
      AccountName owner;

      /**
      * eos file path
      **/
      String eospath;

      /**
      * If to retain link in system 
      **/
      uint8_t keep_link;
  };

  struct PACKED( Store ) {
     /**
     * account name of producer
     **/
     AccountName producer;

     /**
     * account name of file owner
     **/
     AccountName owner;

     /**
     * eos file path
     **/
     String eospath;
  };

  struct PACKED( Reject ) {
     /** 
     * account name of producer
     **/
     AccountName producer;

     /**
     * account name of file owner
     **/
     AccountName owner;

     /**
     * eos file path
     **/
     String eospath;

     /**
     * reason for rejection
     **/
     String reason;
  };

  typedef Table<N(storage),N(storage),N(capacity),Capacity,AccountName> CapacityTable;
  typedef Table<N(storage),N(storage),N(allocations), Allocation, AccountName> AllocationTable;
  typedef VarTable<N(storage),N(storage),N(links), char*> LinkTable;
} /// @} /// storageapi
