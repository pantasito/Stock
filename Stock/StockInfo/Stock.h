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

      std::vector <Object::Trade> _trade;

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
            auto product_id = order->ProductId();
            auto count      = order->Count();
            auto client_id  = order->ClientId();
      
            if (GetProductAmount(product_id) == 0) {
              _trade.emplace_back(product_id, count, client_id, Object::TradeType::Reject);
            }
            if (GetProductAmount(product_id) < count) {
              _trade.emplace_back(product_id, count, client_id, Object::TradeType::PartFill);
              _products_counts[product_id] = 0;
            }
            if (GetProductAmount(product_id) >= count) {
              _trade.emplace_back(product_id, count, client_id, Object::TradeType::FullFill);
              _products_counts[product_id] -= count;
              //_products_counts[product_id] = GetProductAmount(product_id) - count;
            }
          }
        }

        return true;
      }
    };
  }
}