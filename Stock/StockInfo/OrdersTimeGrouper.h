#pragma once

#include <vector>

#include "../Reader/Reader.h"

namespace Stock
{
  namespace StockInfo
  {
    class OrdersTimeGrouper
    {
      std::unique_ptr<Reader::BinaryReader> _reader;

      OrdersTimeGrouper(const OrdersTimeGrouper&) = delete;
      void operator=(const OrdersTimeGrouper&) = delete;

      std::vector<std::unique_ptr<Object::Order>> _orders_group;
      std::unique_ptr<Object::Order> _prepared_order;

    public:
      OrdersTimeGrouper(std::unique_ptr<Reader::BinaryReader> reader)
        : _reader(move(reader))
        {
          _prepared_order = _reader->GetOrder();
        };

      std::vector<std::unique_ptr<Object::Order>> GetGroup() {
        if (_prepared_order == nullptr) {
          if (!_orders_group.empty()) {
            throw std::logic_error("Array is not empty");
          }
          return std::move(_orders_group);
        }

        const uint64_t cur_group_time = _prepared_order->Time();

        _orders_group.emplace_back(std::move(_prepared_order));
        _prepared_order = _reader->GetOrder();

        while (_prepared_order != nullptr && _prepared_order->Time() == cur_group_time) {
          _orders_group.emplace_back(std::move(_prepared_order));
          _prepared_order = _reader->GetOrder();
        }

        return std::move(_orders_group);
      };
    };
  }
}
