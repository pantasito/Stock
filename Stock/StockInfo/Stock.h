#pragma once

#include <map>
#include <vector>

#include <algorithm>


#include "../Object/Order.h"
#include "../Object/Trade.h"
#include "OrdersTimeGrouper.h"

namespace Stock
{
  namespace StockInfo
  {
    class Stock {
      static const int kClientsActivityTimeDelta = 10;


      std::map<uint64_t, uint32_t> _products_counts; // Ключем является product_id, значением кол-во товара

      std::unique_ptr <StockInfo::OrdersTimeGrouper> _grouper;

      std::vector<std::unique_ptr<Object::Trade>> _trades;

      void AnalyzeBuyOrder(const std::unique_ptr <Object::Order> &order ) {
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
        }

        _trades.emplace_back(std::make_unique<Object::Trade>(product_id, count, client_id, type));
      }

    public:
      Stock(std::unique_ptr <StockInfo::OrdersTimeGrouper> grouper)
        : _grouper(move(grouper))
      {}

      uint32_t GetProductAmount(uint64_t product_id) {
        const auto product_it = _products_counts.find(product_id);
        return (product_it == _products_counts.end() ? 0 : product_it->second);
      }

      // Просит новую группу и анализирует ее. Возвращает false если группы кончились
      bool AnalyzeNewGroup() {
        const auto orders_group = _grouper->GetGroup();

        if (orders_group.empty()) {
          return false;
        }   

        for (const auto& order : orders_group) {
          if (order->Type() == Object::OrderType::Create) {
            const auto succes_emplace = _products_counts.emplace(order->ProductId(), order->Count()).second;
            if (!succes_emplace) {
              throw std::logic_error("Product already exist");
            }
          }
        }

        for (const auto& order : orders_group) {
          if (order->Type() == Object::OrderType::Remove) {
            _products_counts.erase(order->ProductId());
          }
        }

        for (const auto& order : orders_group) {
          if (order->Type() == Object::OrderType::Buy) {
            AnalyzeBuyOrder(order);
          }
        }

        return true;
      }
    };
  }
}