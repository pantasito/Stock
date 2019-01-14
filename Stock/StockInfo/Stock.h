#pragma once

#include <map>
#include <vector>

#include <algorithm>

#include "../Object/Order.h"
#include "../Object/Trade.h"
#include "OrdersTimeGrouper.h"
#include "ClientActivity.h"

namespace Stock
{
  namespace StockInfo
  {
    class Stock {
      std::map<uint64_t, uint32_t> _products_counts; // Ключем является product_id, значением кол-во товара

      std::unique_ptr <StockInfo::OrdersTimeGrouper> _grouper;

      std::vector<std::unique_ptr<Object::Trade>> _trades;

      ClientsActivity _clients_activity;

      void AnalyzeBuyOrder(const std::unique_ptr<Object::Order>& order) {
        const auto product_id = order->ProductId();
        const auto count = order->Count();
        const auto client_id = order->ClientId();

        auto it = _products_counts.find(product_id);
        Object::TradeType type;

        if (it == _products_counts.end()) {
          type = Object::TradeType::Reject;
        }
        else if (it->second < count) {
          type = Object::TradeType::PartFill;
          _products_counts.erase(it);
        }
        else {
          type = Object::TradeType::FullFill;
          it->second -= count;
          if (it->second == 0) {
            _products_counts.erase(it);
          }
        }
        
        _clients_activity.AddTradedOrder(client_id, order->Time(), count);
        _trades.emplace_back(std::make_unique<Object::Trade>(product_id, count, client_id, type));
      }

    public:
      Stock(std::unique_ptr <StockInfo::OrdersTimeGrouper> grouper)
        : _grouper(move(grouper))
      {}

      //Возвращает количество товара, 0, если запрашиваемого id нет
      uint32_t GetProductAmount(uint64_t product_id) const {
        const auto product_it = _products_counts.find(product_id);
        return (product_it == _products_counts.end() ? 0 : product_it->second);
      }

      //Просит новую группу и анализирует ее. Возвращает false если группы кончились
      bool AnalyzeNewGroup() {
        auto orders_group = _grouper->GetGroup();

        if (orders_group.empty()) {
          return false;
        }   
       
        //в конце вектора лежат упорядоченные по возрастанию заказы типа Buy
        const auto it_to_buyes = std::stable_partition(orders_group.begin(), orders_group.end(), [](auto& order) { return order->Type() != Object::OrderType::Buy; });
        
        std::stable_sort(it_to_buyes, orders_group.end(),
        [this](const auto& a,const auto& b) {
          return _clients_activity.GetCount(a->ClientId(), a->Time()) < _clients_activity.GetCount(b->ClientId(), b->Time());
        });
        
        //в начале лежат заказы типа Create
        const auto it_to_delete = std::stable_partition(orders_group.begin(), it_to_buyes, [](const auto& order) { return order->Type() == Object::OrderType::Create; });
       
        //прохожу по заказам создать - создаю
        for (auto it = orders_group.begin(); it != it_to_delete; ++it) {
          const auto success_emplace = _products_counts.emplace((*it)->ProductId(), (*it)->Count()).second;
          if (!success_emplace) {
            throw std::logic_error("Product already exist");
          }
        }

        //прохожу по заказам удалить - удаляю
        for (auto it = it_to_delete; it != it_to_buyes; ++it) {
          _products_counts.erase((*it)->ProductId());
        }

        //прохожу по заказам bye - обрабатываю
        for (auto it = it_to_buyes; it != orders_group.end(); ++it) {
          if ((*it)->ClientId() > kMaxNumberOfClients) {
            throw std::logic_error("Number of client is out of range");
          }

          AnalyzeBuyOrder(*it);
        }

        return true;
      }

      const auto& Trades() const {
        return _trades;
      }

    };
  }
}
