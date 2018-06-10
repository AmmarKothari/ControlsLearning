#include <iostream>
#include <iomanip>
#include <vector>

#include "test_utils.h"


int main() {
  std::cout << ten() << std::endl;
  std::vector<int> TENS;
  TENS = tens();
  for (int i = 0; i != TENS.size(); ++i) {
    std::cout << TENS[i] << std::endl;
  }
  return 1;
}
