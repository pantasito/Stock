#include <iostream>
#include <fstream>

#include "Reader/Reader.h"
#include "Object/Order.h"

//using namespace std;

/*

1) ����������� ������� GetOrder;
2) ������� 
  std::string _missed_orders_file_path;
  std::string _orders_file_path;

3) ����������� ��������� �����, ������� ����� ����������� ������������ ������ BinaryReader
�.�. ��, ��� ������� GetOrder ������ ������ � ������� ����������� ������� � 1.
� ����� � ������� ���, ��� � ���� ����� ��� ���������

*/

using namespace Stock;

int main() {  
  
//  checre();
  Reader::BinaryReader r("reserve.bin","out.bin");
  r.print();


  // ������������ ������
  // auto reader = std::make_unique<Reader::BinaryReader>("reserve.bin","out.bin");
  // ReaderTester reader_tester(std::move(reader));
  // bool success = reader_tester.test(); 



  system("pause");
}
