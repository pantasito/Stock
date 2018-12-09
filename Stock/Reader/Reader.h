#pragma once
#include <iostream>
#include <fstream>

#include <map>
#include <string>

#include <memory>

#include "Object/Order.h"

namespace Stock
{
  namespace Reader
  {
    class BinaryReader
    {
      class MissedOrders
      {
        std::map<uint64_t, std::unique_ptr<Object::Order>> _missed_orders_map;
        std::map<uint64_t, std::unique_ptr<Object::Order>>::iterator _cur_missed_order_to_give_it;
      
      public:
        MissedOrders(const std::string& missed_orders_file_path) {
          std::ifstream missed_orders_ifs(missed_orders_file_path, std::ios::binary);

          uint64_t  seq_id;
          uint64_t  product_id;
          uint64_t  time;
          Object::OrderType type;
          uint32_t  count;
          uint64_t  client_id;

          while (missed_orders_ifs) {
            missed_orders_ifs.read((char*)&seq_id, sizeof(seq_id));
            if (seq_id == 0) {
              break;
            }

            missed_orders_ifs.read((char*)&product_id, sizeof(product_id));
            missed_orders_ifs.read((char*)&time, sizeof(time));
            missed_orders_ifs.read((char*)&type, sizeof(type));
            missed_orders_ifs.read((char*)&count, sizeof(count));
            missed_orders_ifs.read((char*)&client_id, sizeof(client_id));

            _missed_orders_map.emplace(seq_id, std::make_unique<Object::Order>(product_id, time, type, count, client_id));
          }
          missed_orders_ifs.close();

          _cur_missed_order_to_give_it = _missed_orders_map.begin();
        }

        bool IsOrdersOver() { return _cur_missed_order_to_give_it == _missed_orders_map.end(); }
        
        std::unique_ptr<Object::Order> GetOrderFromMissed(uint64_t seq_id) {

          if (_cur_missed_order_to_give_it == _missed_orders_map.end()) {
            throw std::logic_error("Missed orders is empty");
          }

          if (_cur_missed_order_to_give_it->first != seq_id) {
            throw std::logic_error("Wrong seq_id requested");
          }

          auto result = _cur_missed_order_to_give_it;

          ++_cur_missed_order_to_give_it;
         
          return std::move(result->second);
        }
      };

      MissedOrders _missed_orders;

      // seq_id ордера, который был прочитан, но еще не должен быть выдан
      // Если он 0, то у нас нет ордера в ожидании
      uint64_t _waiting_seq_id;
      std::unique_ptr<Object::Order> _waiting_order;

      // Seq ордера, который должен быть выдан
      uint64_t _expected_seq_id = 1;

      std::ifstream _orders_ifs;

      bool _is_main_ifs_end = false;

      BinaryReader(const BinaryReader&) = delete;
      void operator=(const BinaryReader &) = delete;

      std::unique_ptr<Object::Order> GetOrderFromIfs() {
        // Читает из основного потока данные об ордере, создает и возвращает его

        uint64_t  product_id;
        uint64_t  time;
        Object::OrderType type;
        uint32_t  count;
        uint64_t  client_id;

        _orders_ifs.read((char*)&product_id, sizeof(product_id));
        _orders_ifs.read((char*)&time, sizeof(time));
        _orders_ifs.read((char*)&type, sizeof(type));
        _orders_ifs.read((char*)&count, sizeof(count));
        _orders_ifs.read((char*)&client_id, sizeof(client_id));

        return std::make_unique<Object::Order>(product_id, time, type, count, client_id);
      }

      // Выдает ордер и увеличивает expected
      std::unique_ptr<Object::Order>& ToGiveOrder(std::unique_ptr<Object::Order>& order)
      {
        ++_expected_seq_id;
        return order;
      }

    public:
      BinaryReader(const std::string& orders_file_path, const std::string& missed_orders_file_path)
        : _missed_orders(missed_orders_file_path),
          _orders_ifs(orders_file_path, std::ios::binary)
      {}

      std::unique_ptr<Object::Order> GetOrder() {
        // Если кончились оба потока - возвращаю nullptr
        if (_is_main_ifs_end && _missed_orders.IsOrdersOver()) {
          return nullptr;
        }
        
        // Если кончился основной поток, то "Выдадть ордер из резерва"
        if (_is_main_ifs_end) {
          auto order_to_give = _missed_orders.GetOrderFromMissed(_expected_seq_id);
          return std::move(ToGiveOrder(order_to_give));
        }

        // Работа с ордером, стоящим в ожидании
        if (_waiting_order != nullptr) {

          if (_waiting_seq_id > _expected_seq_id) {
            auto order_to_give = _missed_orders.GetOrderFromMissed(_expected_seq_id);
            return std::move(ToGiveOrder(order_to_give));
          }
          else if (_waiting_seq_id == _expected_seq_id) {
            _waiting_seq_id = 0;
            return std::move(ToGiveOrder(_waiting_order));
          }
          else {
            throw std::logic_error("Waiting seq < expected seq");
          }
        }

        uint64_t seq_id;
        _orders_ifs.read((char*)&seq_id, sizeof(seq_id));

        // Сигнал о том, что основной поток кончился
        if (seq_id == 0) {
          _is_main_ifs_end = true;
          
          if (!_missed_orders.IsOrdersOver()) {
            auto order_to_give = _missed_orders.GetOrderFromMissed(_expected_seq_id);
            return std::move(ToGiveOrder(order_to_give));
          }
          else {
            return nullptr;
          }
        }

        if (seq_id < _expected_seq_id) {
          throw std::logic_error("Error with seq_id's");
        }

        // Считывается сам ордер формируешь юник поинтер
        auto order = GetOrderFromIfs();

        if (seq_id == _expected_seq_id) {
          return std::move(ToGiveOrder(order));
        }
        else { // (seq_id > _expected_seq_id)          
          _waiting_seq_id = seq_id;
          _waiting_order = std::move(order);

          auto order_to_give = _missed_orders.GetOrderFromMissed(_expected_seq_id);
          return std::move(ToGiveOrder(order_to_give));
        }
      }
    };
  }
}