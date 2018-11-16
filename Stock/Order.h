#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
using namespace std;

const char* missed_orders_file_path = "C:\\VSP\\AndrewTasks\\Stock\\Stock\\Stock\\reserve.bin";

class Order
{
  uint64_t  _seq_id; // SeqId()
  uint64_t  _product_id;
  uint64_t  _time;
  char      _type;
  uint32_t  _count;
  uint64_t  _client_id;

public:
  Order(ifstream& ifs) {
    ifs.read((char*)this, sizeof(Order));
  }

  Order(const Order&) = delete;
  void operator=(const Order&) = delete;

  uint64_t GetSeqId() const {
    return _seq_id;
  }

  void print() {
    cout << _seq_id << " " << _product_id << " " << _time << " " << (int)_type << " " << _count << " " << _client_id << endl;
  }
};

void RemoveLastElem(vector<Order*>& vector) {
  delete vector.back();
  vector.pop_back();
}

void DeleteIncorrectlyRead(vector<Order*>& vector) {
  while (vector.back()->GetSeqId() != 0) {
    RemoveLastElem(vector);
  }
  RemoveLastElem(vector);
}


vector <Order*> GetMissedOrders() {
  vector<Order*> result_orders;

  ifstream missed_orders_ifs(missed_orders_file_path, ios::binary);

  while (missed_orders_ifs) {
    result_orders.push_back(new Order(missed_orders_ifs));
  }

  DeleteIncorrectlyRead(result_orders);

  sort(result_orders.begin(), result_orders.end(), [](const Order* lhs, const Order* rhs)
  {return lhs->GetSeqId() < rhs->GetSeqId(); });

  return result_orders;
}