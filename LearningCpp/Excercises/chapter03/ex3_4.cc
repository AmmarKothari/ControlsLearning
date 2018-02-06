#include <ios>
#include <iostream>
#include <vector>
#include <string>


using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector;


int main() {
	cout << "Enter a string.  "
			"I'll tell you the length of the longest and shortest word!"
			<< endl;

	vector<string> all_words;

	string x;
	while (cin >> x) {
		all_words.push_back(x);
	}
	cout << endl;


	vector<int> word_lengths(2, all_words[0].size()); // initialize to length of first word
	for (int i = 1; i!=all_words.size(); ++i) {
		if (word_lengths[0] > all_words[i].size()) {
			word_lengths[0] = all_words[i].size();
		}
		if (word_lengths[1] < all_words[i].size()) {
			word_lengths[1] = all_words[i].size();
		}
		// cout << "Word: " << all_words[i] << endl;
	}
	cout << "Longest Word: " << word_lengths[1] << endl;
	cout << "Shortest Word: " << word_lengths[0] << endl;






	return 0; // success
}