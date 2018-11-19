#pragma once
#include <iostream>

namespace Stock
{
  namespace Object
  {
    enum class OrderType : char
    {
      Create = 0,
      Buy = 1,
      Remove = 2
    };

    class Order {
      uint64_t  _product_id;
      uint64_t  _time;
      OrderType _type;
      uint32_t  _count;
      uint64_t  _client_id;

      Order(const Order&) = delete;
      void operator=(const Order&) = delete;

    public:
      Order(uint64_t product_id, uint64_t  time, OrderType type, uint32_t  count, uint64_t  client_id)
        :_product_id(product_id),
        _time(time),
        _type(type),
        _count(count),
        _client_id(client_id)
      {}

      void print() {
        std::cout << _product_id << " " << _time << " " << (int)_type << " " << _count << " " << _client_id << std::endl;
      }
    };
  }
}