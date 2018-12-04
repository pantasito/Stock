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

  // ������������ ������
   auto reader = std::make_unique<Reader::BinaryReader>("out.bin", "reserve.bin");
   ReaderTester reader_tester(std::move(reader));
   bool success = reader_tester.test(); 

  system("pause");
}
