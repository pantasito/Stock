#pragma once
#include <iostream>
#include <fstream>

#include <map>
#include <string>

#include <memory>

#include "Object/Order.h"

using namespace std;

class Reader
{
  map<uint64_t, unique_ptr<Order>> _missed_orders;
  int _expected_seq_id = 0;
  string _missed_orders_file_path;
  string _orders_file_path;

  void MissedOrdersPreparation() {
    ifstream missed_orders_ifs(_missed_orders_file_path);
    while (missed_orders_ifs) {
      uint64_t seq_id;
      missed_orders_ifs.read((char*)&seq_id, sizeof(seq_id));
      if (seq_id == 0) break;
      _missed_orders[seq_id] = make_unique<Order>(missed_orders_ifs);
    }
    missed_orders_ifs.close();
  }

  Reader(const Reader&) = delete;

  void operator=(const Reader &) = delete;

public:
  Reader(string missed_orders_file_path, string orders_file_path) {
    _missed_orders_file_path = missed_orders_file_path;
    _orders_file_path = orders_file_path;
    MissedOrdersPreparation();
  }
  
  void print () {
    auto it = _missed_orders.begin();
    auto it_end = _missed_orders.end();
    while (it != it_end) {
      cout << (*it).first << " "; 
      (*(*it).second).print();
      ++it;
    }
  }
  
  Order* GetOrder() {
    ifstream orders_ifs(_orders_file_path);
    uint64_t seq_id;
    
    orders_ifs.read((char*)&seq_id, sizeof(seq_id));
    while (seq_id != _expected_seq_id)
    {

    }

  }
};

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

