#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
using namespace std;

const char* missed_orders_file_path = "D:\\C++ projects\\Andre\\SergeyStock\\Stock\\Debug\\reserve.bin";
const char* orders_file_path = "D:\\C++ projects\\Andre\\SergeyStock\\Stock\\Debug\\out.bin";
/*
enum class OrderType
{
  AllCompleted = 0,
  PartCompleted = 1,
  Rejected = 2
};
*/

// 
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
  
  void print() {
    cout <<_seq_id << " " << _product_id << " " << _time << " " << (int)_type << " " << _count << " " << _client_id << endl;
  }
};


class Reader
{

public:
  Reader(string a, string b) 
  {
    // —читать файл б
    // —обрать из него вектор
    // отсортировать

    // ќ“крыть поток
  }

  Order* GetOrder() {} 
};


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

vector<Order> GetMissedOrders() {
  vector<Order> result_orders;

  ifstream missed_orders_ifs(missed_orders_list, ios::binary);

  while (missed_orders_ifs) {
  //  Order n;
   // orders.read((char*)&n, sizeof(n));
    //Order n(orders);
    //res.push_back(n);
    result_orders.emplace_back(missed_orders_ifs); // —оздает объект пр€мо внутри вектора, на вход она принимает аргументы конструктора
  }

  result_orders.close();
  return result_orders;
}                       

int main() {  
  checre();
  vector<Order> missed_orders = GetMissedOrders();

  /*
  for (auto item : missed_orders) {
    item.print();
  }
  */



  system("pause");
}
