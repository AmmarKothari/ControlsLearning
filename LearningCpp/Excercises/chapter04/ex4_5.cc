#include <iostream>
#include <vector>
#include <iomanip>

#include "ex4_utils.h"

using std::cin; using std::cout; using std::endl;
using std::left; using std::right;
using std::string;
using std::vector;
using std::setw;




int main() {
  cout << "Enter a string.  I'll tell you how many words are in the input and how many times each word appears." << endl;
  vector<string> inputs;
  inputs = store_input();
  print_input(inputs);
  print_input_length(inputs);
  vector<string> unique = unique_words(inputs);
  vector<int> unique_words_count = unique_count(inputs, unique);
  int longest_length = longest_word(inputs);
  for (int i = 0; i < unique.size(); ++i){
    cout << setw(longest_length) << left << unique[i] << setw(4) << right << unique_words_count[i] << endl;
    // cout << inputs[i] << endl;
  }


}
