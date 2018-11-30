#pragma once

#include "../StockData/Stock.h"
#include "../Object/Order.h"
#include "ClientsFactory.h"

#include <memory>

#include <vector>
#include <queue>

namespace Stock
{
  namespace Client
  {
    class ClientsBroker
    {
      std::vector<std::unique_ptr<BaseClient>> _clients;
      std::shared_ptr<StockData::Stock> _stock;

      std::queue<std::unique_ptr<Object::Order>> _ready_orders;

    public:
      ClientsBroker(std::shared_ptr<StockData::Stock> stock)
        :_stock(std::move(stock))
      {}

      std::unique_ptr<Object::Order> GetOrder();

      void ClientsBroker::AddClient(std::unique_ptr<BaseClient> client) { _clients.emplace_back(std::move(client)); }
    };
  }
}