#pragma once
#include "storage.hpp"
#include <eoslib/eos.hpp>
#include <eoslib/raw.hpp>
#include <eoslib/memory.hpp>
#include <eoslib/datastream.hpp>

/**
 *  @defgroup examplecontract Example Storage
 *  @brief Example Contract
 *  @ingroup contractapi
 *
 */
using namespace storage;

namespace eos {
  /**
  * Method to unpack types from character buffer
  **/
  template<typename T>
  T unpack(eos::datastream<char*>& ds) {
      T t;
      eos::raw::unpack(ds, t);
      return t;
  }

  /**
  * Methods to convert from types to bytes
  **/
  template<typename T>
  Bytes valueToBytes(const T& t) {
       uint32_t maxsize = sizeof(t);
       char* buffer = (char*)eos::malloc(maxsize);
       eos::datastream<char*> ds(buffer, maxsize);
       eos::raw::pack(ds, t);

       Bytes bytes;
       bytes.len = ds.tellp();
       bytes.data = (uint8_t*)buffer;
       return bytes;
  }

  /**
  * Method to convert from bytes to types
  **/
  template<typename T>
  T bytesToValue(const Bytes& bytes) { return *reinterpret_cast<T*>(bytes.data); }



  void toBytes(const eos::datastream<char*>& ds, Bytes& bytes, char* buffer)
  {
      bytes.len = ds.tellp();
      bytes.data = (uint8_t*)buffer;
  }

  template<>
  Transfer unpack<Transfer>(eos::datastream<char*>& ds) {
      Transfer transfer;
      eos::raw::unpack(ds, transfer.from);
      eos::raw::unpack(ds, transfer.to);
      eos::raw::unpack(ds, transfer.amount);
      eos::raw::unpack(ds, transfer.memo);
      return transfer;
  }


  template<>
  Transfer bytesToValue<Transfer>(const Bytes& bytes) {
      eos::datastream<char*> ds((char*)bytes.data, bytes.len);
      return unpack<Transfer>(ds);
  }


  template<>
  Bytes valueToBytes<Transfer>(const Transfer& t) {
      uint32_t maxsize = 0;
      maxsize += sizeof(t.from);
      maxsize += sizeof(t.to);
      maxsize += sizeof(t.amount);
      maxsize += t.memo.len + 4;

      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);

      eos::raw::pack(ds, t.from);
      eos::raw::pack(ds, t.to);
      eos::raw::pack(ds, t.amount);
      eos::raw::pack(ds, t.memo);

      Bytes bytes;
      toBytes(ds, bytes, buffer);

      return bytes;
  }

  /**
  * methods to read types from message
  **/
  template<>
  Transfer currentMessage<Transfer>() {
      uint32_t msgsize = messageSize();
      char * buffer = (char*)eos::malloc(msgsize);
      assert(readMessage(buffer, msgsize) == msgsize, "error reading transfer message");
      eos::datastream<char*> ds(buffer, msgsize);
      Transfer transfer = unpack<Transfer>(ds);
      eos::free(buffer);
      return transfer;
  }


    template<>
    Capacity unpack<Capacity>(eos::datastream<char*>& ds) {
        Capacity capacity;
        eos::raw::unpack(ds, capacity.owner);
        eos::raw::unpack(ds, capacity.supply);
        eos::raw::unpack(ds, capacity.reserve);
        return capacity;
    }


    template<>
    Capacity bytesToValue<Capacity>(const Bytes& bytes) {
        eos::datastream<char*> ds((char*)bytes.data, bytes.len);
        return unpack<Capacity>(ds);
    }


    template<>
    Bytes valueToBytes<Capacity>(const Capacity& t) {
        uint32_t maxsize = 0;
        maxsize += sizeof(t.owner);
        maxsize += sizeof(t.supply);
        maxsize += sizeof(t.reserve);

        char* buffer = (char*)eos::malloc(maxsize);
        eos::datastream<char*> ds(buffer, maxsize);

        eos::raw::pack(ds, t.owner);
        eos::raw::pack(ds, t.supply);
        eos::raw::pack(ds, t.reserve);

        Bytes bytes;
        toBytes(ds, bytes, buffer);

        return bytes;
    }

    /**
    * methods to read types from message
    **/
    template<>
    Capacity currentMessage<Capacity>() {
        uint32_t msgsize = messageSize();
        char * buffer = (char*)eos::malloc(msgsize);
        assert(readMessage(buffer, msgsize) == msgsize, "error reading capacity message");
        eos::datastream<char*> ds(buffer, msgsize);
        Capacity capacity = unpack<Capacity>(ds);
        eos::free(buffer);
        return capacity;
    }

  template<>
  Allocation unpack<Allocation>(eos::datastream<char*>& ds) {
      Allocation allocation;
      eos::raw::unpack(ds, allocation.account);
      eos::raw::unpack(ds, allocation.percentAllocated);
      eos::raw::unpack(ds, allocation.usedBytes);
      eos::raw::unpack(ds, allocation.totalPaid);

      return allocation;
  }


  template<>
  Allocation bytesToValue<Allocation>(const Bytes& bytes) {
      eos::datastream<char*> ds((char*)bytes.data, bytes.len);
      return unpack<Allocation>(ds);
  }


  template<>
  Bytes valueToBytes<Allocation>(const Allocation& t) {
      uint32_t maxsize = 0;
      maxsize += sizeof(t.account);
      maxsize += sizeof(t.percentAllocated);
      maxsize += sizeof(t.usedBytes);
      maxsize += sizeof(t.totalPaid);

      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);

      eos::raw::pack(ds, t.account);
      eos::raw::pack(ds, t.percentAllocated);
      eos::raw::pack(ds, t.usedBytes);
      eos::raw::pack(ds, t.totalPaid);

      Bytes bytes;
      toBytes(ds, bytes, buffer);

      return bytes;
  }

  /**
  * methods to read types from message
  **/
  template<>
  Allocation currentMessage<Allocation>() {
      uint32_t msgsize = messageSize();
      char * buffer = (char*)eos::malloc(msgsize);
      assert(readMessage(buffer, msgsize) == msgsize, "error reading allocation message");
      eos::datastream<char*> ds(buffer, msgsize);
      Allocation allocation = unpack<Allocation>(ds);
      eos::free(buffer);
      return allocation;
  }


  template<>
  Link unpack<Link>(eos::datastream<char*>& ds) {
      Link link;
      eos::raw::unpack(ds, link.owner);
      eos::raw::unpack(ds, link.eospath);
      eos::raw::unpack(ds, link.ipfspath);
      eos::raw::unpack(ds, link.size);
      eos::raw::unpack(ds, link.requestHosting);
      eos::raw::unpack(ds, link.accepted);
      eos::raw::unpack(ds, link.producer);

      return link;
  }


  template<>
  Link bytesToValue<Link>(const Bytes& bytes) {
      eos::datastream<char*> ds((char*)bytes.data, bytes.len);
      return unpack<Link>(ds);
  }
  
  template<>
  Bytes valueToBytes<String>(const String& str) {
      uint32_t maxsize = str.len + 4;
      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);
      eos::raw::pack(ds, str);
      Bytes bytes;
      toBytes(ds, bytes, buffer);
      return bytes;
  }

  template<>
  Bytes valueToBytes<Link>(const Link& t) {
      uint32_t maxsize = 0;
      maxsize += sizeof(t.owner);
      maxsize += t.eospath.len + 4;
      maxsize += t.ipfspath.len + 4;
      maxsize += sizeof(t.size);
      maxsize += sizeof(t.requestHosting);
      maxsize += sizeof(t.accepted);
      maxsize += sizeof(t.producer);

      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);

      eos::raw::pack(ds, t.owner);
      eos::raw::pack(ds, t.eospath);
      eos::raw::pack(ds, t.ipfspath);
      eos::raw::pack(ds, t.size);
      eos::raw::pack(ds, t.requestHosting);
      eos::raw::pack(ds, t.accepted);
      eos::raw::pack(ds, t.producer);

      Bytes bytes;
      toBytes(ds, bytes, buffer);

      return bytes;
  }

  /**
  * methods to read types from message
  **/
  template<>
  Link currentMessage<Link>() {
      uint32_t msgsize = messageSize();
      char * buffer = (char*)eos::malloc(msgsize);
      assert(readMessage(buffer, msgsize) == msgsize, "error reading link message");
      eos::datastream<char*> ds(buffer, msgsize);
      Link link = unpack<Link>(ds);
      eos::free(buffer);
      return link;
  }


  template<>
  RemoveLink unpack<RemoveLink>(eos::datastream<char*>& ds) {
      RemoveLink removeLink;
      eos::raw::unpack(ds, removeLink.owner);
      eos::raw::unpack(ds, removeLink.eospath);
      eos::raw::unpack(ds, removeLink.keep_link);

      return removeLink;
  }


  template<>
  RemoveLink bytesToValue<RemoveLink>(const Bytes& bytes) {
      eos::datastream<char*> ds((char*)bytes.data, bytes.len);
      return unpack<RemoveLink>(ds);
  }

  template<>
  Bytes valueToBytes<RemoveLink>(const RemoveLink& t) {
      uint32_t maxsize = 0;
      maxsize += sizeof(t.owner);
      maxsize += t.eospath.len + 4;
      maxsize += sizeof(t.keep_link);

      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);

      eos::raw::pack(ds, t.owner);
      eos::raw::pack(ds, t.eospath);
      eos::raw::pack(ds, t.keep_link);

      Bytes bytes;
      toBytes(ds, bytes, buffer);

      return bytes;
  }

  /**
  * methods to read types from message
  **/
  template<>
  RemoveLink currentMessage<RemoveLink>() {
      uint32_t msgsize = messageSize();
      char * buffer = (char*)eos::malloc(msgsize);
      assert(readMessage(buffer, msgsize) == msgsize, "error reading removeLink message");
      eos::datastream<char*> ds(buffer, msgsize);
      RemoveLink removeLink = unpack<RemoveLink>(ds);
      eos::free(buffer);
      return removeLink;
  }



  template<>
  Store unpack<Store>(eos::datastream<char*>& ds) {
      Store store;
      eos::raw::unpack(ds, store.producer);
      eos::raw::unpack(ds, store.owner);
      eos::raw::unpack(ds, store.eospath);

      return store;
  }


  template<>
  Store bytesToValue<Store>(const Bytes& bytes) {
      eos::datastream<char*> ds((char*)bytes.data, bytes.len);
      return unpack<Store>(ds);
  }

  template<>
  Bytes valueToBytes<Store>(const Store& t) {
      uint32_t maxsize = 0;
      maxsize += sizeof(t.producer);
      maxsize += sizeof(t.owner);
      maxsize += t.eospath.len + 4;

      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);

      eos::raw::pack(ds, t.producer);
      eos::raw::pack(ds, t.owner);
      eos::raw::pack(ds, t.eospath);

      Bytes bytes;
      toBytes(ds, bytes, buffer);

      return bytes;
  }

  /**
  * methods to read types from message
  **/
  template<>
  Store currentMessage<Store>() {
      uint32_t msgsize = messageSize();
      char * buffer = (char*)eos::malloc(msgsize);
      assert(readMessage(buffer, msgsize) == msgsize, "error reading store message");
      eos::datastream<char*> ds(buffer, msgsize);
      Store store = unpack<Store>(ds);
      eos::free(buffer);
      return store;
  }



  template<>
  Reject unpack<Reject>(eos::datastream<char*>& ds) {
      Reject reject;
      eos::raw::unpack(ds, reject.producer);
      eos::raw::unpack(ds, reject.owner);
      eos::raw::unpack(ds, reject.eospath);
      eos::raw::unpack(ds, reject.reason);

      return reject;
  }


  template<>
  Reject bytesToValue<Reject>(const Bytes& bytes) {
      eos::datastream<char*> ds((char*)bytes.data, bytes.len);
      return unpack<Reject>(ds);
  }

  template<>
  Bytes valueToBytes<Reject>(const Reject& t) {
      uint32_t maxsize = 0;
      maxsize += sizeof(t.producer);
      maxsize += sizeof(t.owner);
      maxsize += t.eospath.len + 4;
      maxsize += t.reason.len + 4;

      char* buffer = (char*)eos::malloc(maxsize);
      eos::datastream<char*> ds(buffer, maxsize);

      eos::raw::pack(ds, t.producer);
      eos::raw::pack(ds, t.owner);
      eos::raw::pack(ds, t.eospath);
      eos::raw::pack(ds, t.reason);

      Bytes bytes;
      toBytes(ds, bytes, buffer);

      return bytes;
  }

  /**
  * methods to read types from message
  **/
  template<>
  Reject currentMessage<Reject>() {
      uint32_t msgsize = messageSize();
      char * buffer = (char*)eos::malloc(msgsize);
      assert(readMessage(buffer, msgsize) == msgsize, "error reading reject message");
      eos::datastream<char*> ds(buffer, msgsize);
      Reject reject = unpack<Reject>(ds);
      eos::free(buffer);
      return reject;
  }


}
