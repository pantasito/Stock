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
  std::unique_ptr<Reader::BinaryReader> _reader_tester;

public:
  ReaderTester(std::unique_ptr<Reader::BinaryReader> reader) : _reader_tester(std::move(reader))
  {}

  bool test() {
    std::unique_ptr<Object::Order> tmp(std::move(_reader_tester->GetOrder()));
    
    while (tmp != nullptr) {
      std::unique_ptr<Object::Order> tmp(std::move(_reader_tester->GetOrder()));
    }
    return true;
  }
};



int main() {

  // Тестирование ридера
   auto reader = std::make_unique<Reader::BinaryReader>("out.bin", "reserve.bin");
   ReaderTester reader_tester(std::move(reader));
   bool success = reader_tester.test(); 

  system("pause");
}
