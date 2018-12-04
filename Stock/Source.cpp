#include <iostream>
#include <fstream>

#include "Reader/Reader.h"
#include "Object/Order.h"

//using namespace std;

/*

1) ����������� ������� GetOrder;            //����� +
2) �������                                  //����� +
  std::string _missed_orders_file_path;
  std::string _orders_file_path;

3) ����������� ��������� �����, ������� ����� ����������� ������������ ������ BinaryReader
�.�. ��, ��� ������� GetOrder ������ ������ � ������� ����������� ������� � 1.
� ����� � ������� ���, ��� � ���� ����� ��� ���������

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

  // ������������ ������
   auto reader = std::make_unique<Reader::BinaryReader>("out.bin", "reserve.bin");
   ReaderTester reader_tester(std::move(reader));
   bool success = reader_tester.Test(); 
   std::cout << "Result: " << success << std::endl;


  system("pause");
}
