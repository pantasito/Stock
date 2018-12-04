#include <iostream>
#include <fstream>

#include "Reader/Reader.h"
#include "Object/Order.h"

//using namespace std;

/*

1) Реализовать функцию GetOrder;            //вроде +
2) Выпилть                                  //вроде +
  std::string _missed_orders_file_path;
  std::string _orders_file_path;

3) Реализовать отдельный класс, который будет тестировать правильность работы BinaryReader
т.е. то, что функция GetOrder выдает ордера в порядке возрастания начиная с 1.
В мейне я написал код, как я хочу чтобы это выглядело

*/

using namespace Stock;

class ReaderTester 
{
  std::unique_ptr<Reader::BinaryReader> _tested_reader;

public:
  ReaderTester(std::unique_ptr<Reader::BinaryReader> reader)
    : _tested_reader(std::move(reader))
  {}

  bool Test() {
    auto cur_order = _tested_reader->GetOrder();
    
    while (cur_order != nullptr) {
      cur_order = _tested_reader->GetOrder();
    }
    return true;
  }
};


#include <vector>

int main() {  
  checre();
  vector<Order> missed_orders = GetMissedOrders();

  std::vector<char> v;
  for (int i = 0; i < 100; i++) {
    v.push_back(i);
  }

  /*
  for (auto item : missed_orders) {
    item.print();
  }
  */

  // Тестирование ридера
   auto reader = std::make_unique<Reader::BinaryReader>("out.bin", "reserve.bin");
   ReaderTester reader_tester(std::move(reader));
   bool success = reader_tester.Test(); 
   std::cout << "Result: " << success << std::endl;


  system("pause");
}
