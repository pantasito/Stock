#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
using namespace std;

const char* missed_orders_list = "C:\\VSP\\AndrewTasks\\LikeAtWork\\reserve.bin";
const char* current = "C:\\VSP\\AndrewTasks\\LikeAtWork\\out.bin";
/*
enum class TradeType
{
  AllCompleted = 0,
  PartCompleted = 1,
  Rejected = 2
};
*/

class Trade
{
  uint32_t  _seq_id;
  uint64_t  _product_id;
  uint64_t  _time;
  char      _type;
  uint32_t  _count;
  uint64_t  _client_id;

public:
  Trade() {
    ifstream reserve(current, ios::binary|ios::in);
    reserve.read((char*)this, sizeof Trade);
  }

  Trade(uint32_t  seq_id, uint64_t  product_id, uint64_t time, char type, uint32_t  count, uint64_t  client_id) {
    _seq_id     = seq_id;
    _product_id = product_id;
    _time       = time;
    _type       = type;
    _count      = count;
    _client_id  = client_id;
  }

  void print() {
    cout <<_seq_id << " " << _product_id << " " << _time << " " << (int)_type << " " << _count << " " << _client_id << endl;
  }
};

vector <Trade> GetMissedOrders() {
  vector <Trade> res;

  uint32_t  seq_id = 0;
  uint64_t  product_id = 0;
  uint64_t  time = 0;
  char      type = 0;
  uint32_t  count = 0;
  uint64_t  client_id = 0;

  ifstream orders(missed_orders_list, ios::binary);
  
  orders.read((char*)&seq_id,     sizeof (seq_id));
  orders.read((char*)&product_id, sizeof (product_id));
  orders.read((char*)&time,       sizeof (time));
  orders.read((char*)&type,       sizeof (type));
  orders.read((char*)&count,      sizeof (count));
  orders.read((char*)&client_id,  sizeof (client_id));
  orders.close();

  Trade n(seq_id, product_id, time, type, count, client_id);
  
  res.push_back(n);
  
  return res;           
}                       

int main() {
  
  vector <Trade> missed_orders = GetMissedOrders();
  for (auto item : missed_orders) {
    item.print();
  }
  


  system("pause");
}
