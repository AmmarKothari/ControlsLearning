#include <iostream>
#include <vector>
#include <iomanip>
#include "ex4_utils.h"

using std::cin; using std::cout; using std::endl;
using std::left; using std::right;
using std::string;
using std::vector;
using std::setw;

vector<string> store_input() {
  string x;
  vector<string> inputs;
  while (cin >> x){
    inputs.push_back(x);
  }
  cout << endl;
  return inputs;
}

void print_input(vector<string> inputs){
  cout << "You entered:" << endl;
  for (int i = 0; i != inputs.size(); ++i) {
    cout << inputs[i] << ' ';
  }
  cout << endl;
}

void print_input_length(vector<string> inputs) {
  cout << "Length of Input: " << inputs.size() << endl;
}

int longest_word(vector<string> inputs){
  int longest_length = 0;
  for (int i = 0; i != inputs.size(); ++i){
    if (inputs[i].size() > longest_length) {
      longest_length = inputs[i].size();
    }
  }
  cout << "Length of Longest Word: " << longest_length << endl;
  return longest_length;
}

vector<string> unique_words(vector<string> inputs) {
  vector<string> unique_words;
  vector<int> unique_words_count;
  // build unique collection
  for (int i = 0; i != inputs.size(); ++i) {
    int unique_flag = 1;
    for (int i2 = 0; i2 != unique_words.size(); ++i2) {
      if (inputs[i] == unique_words[i2]) {
        // they are the same
        unique_flag = 0;
        ++unique_words_count[i2];
        continue;
      }
    }
    // they are different
    if (unique_flag == 1){
      unique_words.push_back(inputs[i]);
      unique_words_count.push_back(1);
    }
  }
  return unique_words;
}

vector<int> unique_count(vector<string> input, vector<string> unique_words){
  vector<int> unique_words_count(unique_words.size());
  for (int i2= 0; i2 != unique_words.size(); ++i2){
    for (int i = 0; i != input.size(); ++i){
      if (input[i] == unique_words[i2]) {
        ++unique_words_count[i2];
      }
    }
  }
  return unique_words_count;
}
