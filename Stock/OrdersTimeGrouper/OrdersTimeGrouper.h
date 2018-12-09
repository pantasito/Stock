#pragma once

#include <vector>

#include "Reader/Reader.h"

using namespace Stock;

namespace Stock
{
  namespace OrdersTimeGrouper
  {
    class OrdersTimeGrouper
    {
      std::unique_ptr<Reader::BinaryReader> _reader;

      OrdersTimeGrouper(const OrdersTimeGrouper&) = delete;
      void operator=(const OrdersTimeGrouper&) = delete;

      std::vector<std::unique_ptr<Object::Order >>* _order_group;
      std::unique_ptr<Object::Order > _prepared_order;

    public:
      OrdersTimeGrouper(std::unique_ptr<Reader::BinaryReader> reader) :
        _reader(move(reader))
      {
        _prepared_order = _reader->GetOrder();
      };

      std::vector<std::unique_ptr<Object::Order>>* GetGroup() {
        uint64_t cur_group_time;

        if (_prepared_order == nullptr) {
          return nullptr;
        }

        cur_group_time = _prepared_order->get_time();

        _order_group->push_back(_prepared_order);
        _prepared_order = _reader->GetOrder();

        while (_prepared_order->get_time() == cur_group_time) {
          _order_group->push_back(_prepared_order);
          _prepared_order = _reader->GetOrder();
        }

        return _order_group;
      };
    };
  }
}
