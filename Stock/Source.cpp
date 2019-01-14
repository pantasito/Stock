#include <iostream>
#include <fstream>

#include "Reader/Reader.h"
#include "Object/Order.h"

#include "StockInfo/OrdersTimeGrouper.h"
#include "StockInfo/Stock.h"

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

  // Тестирование ридера
 /*
  auto reader = std::make_unique<Reader::BinaryReader>("out.bin", "reserve.bin");
   ReaderTester reader_tester(std::move(reader));
   bool success = reader_tester.Test(); 
   std::cout << "Result: " << success << std::endl;
   */

  auto reader = std::make_unique<Reader::BinaryReader>("out.bin", "reserve.bin");
  auto grouper = std::make_unique<StockInfo::OrdersTimeGrouper>(std::move(reader));
  StockInfo::Stock stock(std::move(grouper));
  
  while (stock.AnalyzeNewGroup()) {

  }

  for (const auto& trade : stock.Trades()) {
    trade->Print();
  }


  /*
  auto group = grouper.GetGroup();
  auto group_time = group.front()->Time();
  
  while (!group.empty()) {
    for (auto& order : group) {
      if (order->Time() != group_time) {
        throw std::logic_error("G");
      }
    }

    std::cout << group_time << std::endl;

    group = grouper.GetGroup();

    if (group.empty()) {
      break;
    }


    if (group.front()->Time() <= group_time) {
      throw std::logic_error("G");
    }

    group_time = group.front()->Time();
  }

  for (int i = 0; i < 100; i++) {
    if (!grouper.GetGroup().empty()) {
      throw std::logic_error("G");
    }
  }
  */

  system("pause");
}
