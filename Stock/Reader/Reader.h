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
        bool _is_reserve_ifs_end;

        MissedOrders(const std::string& missed_orders_file_path) {
          std::ifstream missed_orders_ifs(missed_orders_file_path);

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

          if (_missed_orders_map.empty()) {
            _is_reserve_ifs_end = true;
            _cur_missed_order_to_give_it = _missed_orders_map.end();
          }
          else {
            _cur_missed_order_to_give_it = _missed_orders_map.begin();
            _is_reserve_ifs_end = false;
          } 
        }

        std::unique_ptr<Object::Order> GetOrderFromMissed(uint64_t  seq_id) {
          // "Выдадть ордер из резерва"
          //        Если кончились ордера в резерве, то сигнализируем ошибку (std::logic_error)
          //        Если _expected_seq_id != что в итераторе (std::logic_error)
          //        Возвращаем указатель 

          if (_cur_missed_order_to_give_it == _missed_orders_map.end()) {
            throw std::logic_error("Missed orders is empty");
          }

          if (_cur_missed_order_to_give_it->first != seq_id) {
            throw std::logic_error("Wrong seq_id requested");
          }
          
          if (_cur_missed_order_to_give_it == (_missed_orders_map.end())--) {   //уродливый код -> _missed_orders_map.end()--  как нужно было?  
            bool _is_reserve_ifs_end = true;
          }
          _cur_missed_order_to_give_it;
          

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

    public:
      BinaryReader(const std::string& orders_file_path, const std::string& missed_orders_file_path)
        : _missed_orders(missed_orders_file_path), _orders_ifs(orders_file_path)
      {}
      
      /* Использовать эту функцию, чтобы возвращать ордера в GetOrder. 
         Например  return std::move(ToGiveOrder(order));

         std::unique_ptr<Object::Order>& ToGiveOrder(std::unique_ptr<Object::Order>& order)
        {
          ++_expected_seq;
          return order;
        }
      */

      std::unique_ptr<Object::Order> GetOrder() {
        // Если кончились оба потока - возвращаю nullptr
        if (_is_main_ifs_end && _missed_orders._is_reserve_ifs_end) { 
          //правильные имена? может _main_ifs_ended, _reserve_ifs_end
          return nullptr;
        }
        
        // Если кончился основной поток, то "Выдадть ордер из резерва"
        if (_is_main_ifs_end) {
          ++_expected_seq_id;
          return _missed_orders.GetOrderFromMissed(_expected_seq_id - 1);
        }

        // Если _waiting_seq_id != 0
        //    Если _waiting_seq_id > _expected_seq_id
        //       "Выдадть ордер из резерва"
        //    Иначе отдаем ордер, который стоит в ожидании
        //    Не забываем занулить _waiting_seq_id

        if (_waiting_seq_id != 0) {
          ++_expected_seq_id;

          if (_waiting_seq_id > _expected_seq_id) {
            return _missed_orders.GetOrderFromMissed(_expected_seq_id - 1);
          }
          else {
            _waiting_seq_id = 0;
            return std::move(_waiting_order); // верно ли это?
          }
        }

        uint64_t seq_id;
        _orders_ifs.read((char*)&seq_id, sizeof(seq_id));

        // Если seq_id == 0
        // "Выдадть ордер из резерва"
        if (seq_id == 0) {
          _is_main_ifs_end = true;
          ++_expected_seq_id;
          return _missed_orders.GetOrderFromMissed(_expected_seq_id - 1);
        }

        // 1) seq_id < _expected_seq_id (проверить, т.к ошибка)
        if (seq_id < _expected_seq_id) {
          throw std::logic_error("Error with seq_id's");
        }

        // Считывается сам ордер формируешь юник поинтер
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

        // 2) seq_id == _expected_seq_id
        //    Возвращаешь ордер
        if (seq_id == _expected_seq_id) {
          ++_expected_seq_id;
          return std::make_unique<Object::Order>(product_id, time, type, count, client_id);
        }

        // 3) seq_id > _expected_seq_id
        //    Появился waiting
        //    "Выдадть ордер из резерва"
        if (seq_id > _expected_seq_id) {
          _waiting_seq_id = seq_id;
          _waiting_order = std::move (std::make_unique<Object::Order> (product_id, time, type, count, client_id));

          ++_expected_seq_id;
          return _missed_orders.GetOrderFromMissed(_expected_seq_id - 1);
        }
      }
    };
  }
}