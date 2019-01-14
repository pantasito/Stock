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

      void AnalyzeBuyOrder(std::vector<std::unique_ptr<Object::Order>>::iterator& order) {
        const auto product_id = (*order)->ProductId();
        const auto count = (*order)->Count();
        const auto client_id = (*order)->ClientId();

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
        //нужно делать add сделки в массив
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
       
        //в конце вектора лежат упорядоченные по возрастанию заказы типа Buy
        auto it_to_buyes = std::stable_partion(orders_group.begin(), orders_group.end(), [](const auto& order) { return order->Type() != Object::OrderType::Buy; });

        std::stable_sort(it_to_buyes, orders_group.end(),
        [this](const auto& a, const auto& b) {
        return _clients_activity.get_count(a.client_id, a.time) < _clients_activity.get_count(b.client_id, b.time)});

        //в начале лежат заказы типа Create
        auto it_to_delete = std::stable_partion(orders_group.begin(), it_to_buyes, [](const auto& order) { return order->Type() == Object::OrderType::Create; });
       
        //прохожу по заказам создать - создаю
        for (auto it_to_create = orders_group.begin(); it_to_create != it_to_delete; ++it_to_create) {
          const auto succes_emplace = _products_counts.emplace((*it_to_create)->ProductId(), (*it_to_create)->Count()).second;
          if (!succes_emplace) {
            throw std::logic_error("Product already exist");
          }
        }

        //прохожу по заказам удалить - удаляю
        for (it_to_delete; it_to_delete != it_to_buyes; ++it_to_delete) {
          _products_counts.erase((*it_to_delete)->ProductId());
        }

        //прохожу по заказам bye - обрабатываю
        for (it_to_buyes; it_to_buyes != orders_group.end(); ++it_to_delete) {
          if ((*it_to_buyes)->ClientId() > kMaxNumberOfClients) throw std::logic_error("Number of client is out of range");
          AnalyzeBuyOrder(it_to_buyes);
        }

        return true;
      }
    };
  }
}