#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include "Reader.h"
#include "Order.h"
using namespace std;

/*
enum class OrderType
{
  AllCompleted = 0,
  PartCompleted = 1,
  Rejected = 2
};
*/

int main() {  
//  checre();
  Reader r;
  Order* tmp = r.GetOrder();
  if (tmp == nullptr) {
    return 0;
  }

  do {
    tmp->print();
    tmp = r.GetOrder();
  } while (tmp != nullptr);

  system("pause");
}
