#include <iostream>
#include <fstream>

#include "Reader/Reader.h"
#include "Object/Order.h"

using namespace std;

int main() {  
//  checre();
  Reader r("C:\\VSP\\AndrewTasks\\Stock\\Stock\\Stock\\reserve.bin", 
           "C:\\VSP\\AndrewTasks\\Stock\\Stock\\Stock\\out.bin");

  r.print();



  system("pause");
}
