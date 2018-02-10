#include <iostream>
#include <vector>
#include <ios>
#include <algorithm>

using std::cin;     using std::cout;
using std::vector;
using std::string;
using std::endl;
using std::getline;

int main() {
  cout << "Type in a string.  I will tell you how many each word appears: ";
  string x;
  vector<string> all_words;
  vector<string> unique_words;
  vector<int> unique_words_count;
  int max_word_length = 0;

  // invariant:  all_words contiains all words read so far
  while (cin >> x) {
    all_words.push_back(x);
  }
  cout << endl;

  // verify that something was entered (all_words != 0)
  typedef vector<string>::size_type string_vec_size;
  string_vec_size all_words_size = all_words.size();
  if (all_words_size == 0) {
    cout << endl << "Nothing Entered" << endl;
    return 1;
  }

  // build unique collection
  for (int i = 0; i != all_words.size(); ++i) {
    int unique = 1;
    for (int i2 = 0; i2 != unique_words.size(); ++i2) {
      if (all_words[i] == unique_words[i2]) {
        // they are the same
        unique = 0;
        ++unique_words_count[i2];
        continue;
      }
    }
    // they are different
    if (unique == 1){
      unique_words.push_back(all_words[i]);
      unique_words_count.push_back(1);
      if (max_word_length < all_words[i].size()) {
        max_word_length = all_words[i].size();
      }
    }

  }
  cout << "Max Word Length: " << max_word_length << endl;
  for (int i = 0; i != unique_words.size(); ++i) {
    cout << "Unique Word: " << unique_words[i];
    for (int s = 0; s != (max_word_length - unique_words[i].size()); ++s) {
      cout << " ";
    }
    cout << " Count: " << unique_words_count[i];
    cout << endl;

  }

  return 0;

}
