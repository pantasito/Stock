#pragma once
#include <iostream>
#include <fstream>
#include "Order.h"
using namespace std;
const char* orders_file_path = "C:\\VSP\\AndrewTasks\\Stock\\Stock\\Stock\\out.bin";

class Reader
{
  vector<Order*> missed_orders;
  ifstream orders_ifs;
  Order* order;
  int expected_seq_id;
  int missed_orders_counter;

public:
  Reader() {
    missed_orders = GetMissedOrders();
    orders_ifs.open(orders_file_path, ios::binary);
    order = new Order(orders_ifs);
    expected_seq_id = 1;
    missed_orders_counter = 0;

  }
  
  Reader(const Reader&) = delete;
  void operator=(const Reader &) = delete;
  
  /*
  Reader(string a, string b)
  {
    // Считать файл б
    // Собрать из него вектор
    // Отсортировать

    // Открыть поток
  }
  */

  Order* GetOrder() {
    if (order->GetSeqId() == 0 && missed_orders_counter >= missed_orders.size()) {
      return nullptr;
    }

    if (order->GetSeqId() == expected_seq_id) {
      expected_seq_id++;
      Order* result = order;
      order = new Order(orders_ifs);
      return result;
    }

    if (missed_orders[missed_orders_counter]->GetSeqId() == expected_seq_id) {
      expected_seq_id++;
      missed_orders_counter++;
      return missed_orders[missed_orders_counter - 1];
    }

    cout << "shirt happens" << endl;
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

