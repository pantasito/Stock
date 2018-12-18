#pragma once

#include <map>

#include "../StockInfo/OrdersTimeGrouper.h"

namespace Stock
{
  class Stock {
    std::map <uint64_t, uint32_t> _stock;
    std::unique_ptr <StockInfo::OrdersTimeGrouper> _grouper;
    std::vector<std::unique_ptr<Object::Order>> _orders_group;

  public:
    Stock(std::unique_ptr <StockInfo::OrdersTimeGrouper> grouper)
      : _grouper(move(grouper))
    {}

    uint32_t GetProductAmount(uint64_t product_id)
    {
      return _stock[product_id];
    }
      
    // Просит новую группу и анализирует ее. Возвращает false если группы кончились
    bool AnalyzeNewGroup() {
      _orders_group = _grouper->GetGroup();

      if (_orders_group.empty()) {
        return false;
      }
        
      for (int i = 0; i < _orders_group.size(); ++i) {
        if (_orders_group[i]->Type() == Stock::Object::OrderType::Crete){
          _stock[_orders_group[i]->ProductId()] = _orders_group[i]->Count();
        }
      }

      for (int i = 0; i < _orders_group.size(); ++i) {
        if (_orders_group[i]->Type() == Stock::Object::OrderType::Remove) {
          _stock[_orders_group[i]->ProductId()] = 0;
        }
      }

      return true;
    }
  };
}