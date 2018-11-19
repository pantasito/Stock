#pragma once
#include <iostream>
#include <fstream>

using namespace std;

enum class OrderType
{
AllCompleted = 0,
PartCompleted = 1,
Rejected = 2
};

class Order {
  uint64_t  _product_id;
  uint64_t  _time;
  OrderType _type;
  uint32_t  _count;
  uint64_t  _client_id;

  Order(const Order&) = delete;
  void operator=(const Order&) = delete;

public:
  Order(ifstream& ifs) {
    ifs.read((char*)&_product_id, sizeof(_product_id));
    ifs.read((char*)&_time, sizeof(_time));
    ifs.read((char*)&_type, sizeof(_type));
    ifs.read((char*)&_count, sizeof(_count));
    ifs.read((char*)&_client_id, sizeof(_client_id));
  }
  
  void print() {
    cout << _product_id << " " << _time << " " << (int)_type << " " << _count << " " << _client_id << endl;
  }
};