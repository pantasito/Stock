#pragma once
#include <iostream>

namespace Stock
{
  namespace Object
  {
    enum class TradeType : char
    {
      FullFill = 0,
      PartFill = 1,
      Reject = 2
    };

    class Trade {
      uint64_t  _product_id;
      uint32_t  _count;
      uint64_t  _client_id;
      TradeType _type;

      Trade(const Trade&) = delete;
      void operator=(const Trade&) = delete;

    public:
      Trade(uint64_t product_id, uint32_t  count, uint64_t  client_id, TradeType type)
        :_product_id(product_id),
        _count(count),
        _client_id(client_id),
        _type(type)
      {}

      uint64_t  ProductId() const { return _product_id; }
      uint32_t  Count()     const { return _count; }
      uint64_t  ClientId()  const { return _client_id; }
      TradeType Type()      const { return _type; }

      void Print() {
        std::cout << _product_id << " " << _count << " " << _client_id << " " << (int)_type << std::endl;
      }
    };
  }
}