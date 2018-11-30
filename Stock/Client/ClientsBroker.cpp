#include "ClientsBroker.h"

namespace Stock
{
  namespace Client
  {
    std::unique_ptr<Object::Order> ClientsBroker::GetOrder()
    {
      if (_ready_orders.empty())
      {
        return nullptr;
      }

      auto order_to_give = std::move(_ready_orders.back());
      _ready_orders.pop();
      return std::move(order_to_give);
    }
  }
}