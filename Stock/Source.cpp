#include <iostream>
#include <fstream>

#include "Reader/Reader.h"
#include "Object/Order.h"

//using namespace std;

/*

1) Реализовать функцию GetOrder;
2) Выпилть 
  std::string _missed_orders_file_path;
  std::string _orders_file_path;

3) Реализовать отдельный класс, который будет тестировать правильность работы BinaryReader
т.е. то, что функция GetOrder выдает ордера в порядке возрастания начиная с 1.
В мейне я написал код, как я хочу чтобы это выглядело

*/

using namespace Stock;

int main() {  
  
//  checre();
  Reader::BinaryReader r("reserve.bin","out.bin");
  r.print();


  // Тестирование ридера
  // auto reader = std::make_unique<Reader::BinaryReader>("reserve.bin","out.bin");
  // ReaderTester reader_tester(std::move(reader));
  // bool success = reader_tester.test(); 



  system("pause");
}
