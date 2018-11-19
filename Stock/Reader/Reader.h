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
      std::map<uint64_t, std::unique_ptr<Object::Order>> _missed_orders;
      std::map<uint64_t, std::unique_ptr<Object::Order>>::iterator _cur_missed_order_to_give_it;

      // seq_id ������, ������� ��� ��������, �� ��� �� ������ ���� ���������
      // ���� �� 0, �� � ��� ��� ������ � ��������
      uint64_t _waiting_seq_id = 0;
      std::unique_ptr<Object::Order> _waiting_order;

      int _expected_seq_id = 0;

      std::ifstream _orders_ifs;

      std::string _missed_orders_file_path;
      std::string _orders_file_path;

      void MissedOrdersPreparation() {
        std::ifstream missed_orders_ifs(_missed_orders_file_path);

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

          //_missed_orders[seq_id] = make_unique<Order>(product_id, time, type, count, client_id);
          _missed_orders.emplace(seq_id, std::make_unique<Object::Order>(product_id, time, type, count, client_id));
        }

        missed_orders_ifs.close();

        if (_missed_orders.empty()) {
          _cur_missed_order_to_give_it = _missed_orders.end();
        }
        else {
          _cur_missed_order_to_give_it = _missed_orders.begin();
        }
      }

      BinaryReader(const BinaryReader&) = delete;
      void operator=(const BinaryReader &) = delete;

    public:
      BinaryReader(const std::string& missed_orders_file_path, const std::string& orders_file_path)
        : _orders_ifs(orders_file_path)
      {
        _missed_orders_file_path = missed_orders_file_path;
        _orders_file_path = orders_file_path;
        MissedOrdersPreparation();
      }

      void print() {
        for (const auto& order : _missed_orders) {
          std::cout << order.first << " ";
          order.second->print();
        }
      }

      // "������� ����� �� �������"
      //        ���� ��������� ������ � �������, �� ������������� ������ (std::logic_error)
      //        ���� _expected_seq_id != ��� � ��������� (std::logic_error)
      //        ���������� ��������� 

      /* ������������ ��� �������, ����� ���������� ������ � GetOrder. 
         ��������  return std::move(ToGiveOrder(order));

         std::unique_ptr<Object::Order>& ToGiveOrder(std::unique_ptr<Object::Order>& order)
        {
          ++_expected_seq;
          return order;
        }
      */

      std::unique_ptr<Object::Order> GetOrder() {
        // ���� �������� �������� �����, �� "������� ����� �� �������"

        // ���� _waiting_seq_id != 0
        //    ���� _waiting_seq_id > _expected_seq_id
        //       "������� ����� �� �������"
        //    ����� ������ �����, ������� ����� � ��������

        uint64_t seq_id;
        _orders_ifs.read((char*)&seq_id, sizeof(seq_id));

        // ���� seq_id == 0
        // "������� ����� �� �������"

        // ����������� ��� ����� ���������� ���� �������

        // 1) seq_id < _expected_seq_id (���������, �.� ������)
        // 2) seq_id == _expected_seq_id
        //    ����������� �����
        // 3) seq_id > _expected_seq_id
        //    �������� waiting
        //    "������� ����� �� �������"
      }
    };
  }
}

/*
void checre()
{
Reader reader("dd", "ff");
int expected_seq_id = 1;
do
{
auto order = reader.GetOrder();
if (order == nullptr)
{
break;
}

if (expected_seq_id != order->_seq_id)
{
throw "BEDA";
}

++expected_seq_id;

} while ()
};
*/

