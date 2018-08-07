// Huffman Coding
//https://en.wikipedia.org/wiki/Huffman_coding

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <typeinfo>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::map;
using std::pair;
using std::vector;
using std::priority_queue;


class Node{
public:
	Node* Left;
	Node* Right;
	string s;
	double freq;
	Node(Node* Left_in, Node* Right_in) {
		Left = Left_in;
		Right = Right_in;
		s = Left->s + Right->s;
		freq = Left->freq + Right->freq;
	}

	Node(string s_in, double freq_in) { // leaf of tree
		Left = NULL;
		Right = NULL;
		s = s_in;
		freq = freq_in;
	}

	Node() {
		;
	}
};


class Comparator {
public:
    bool operator()(const Node* a, const Node* b)
    {
        return (a->freq > b->freq);
    }
};

template<typename T> void print_queue(T q) {
    while(!q.empty()) {
        std::cout << q.top()->s << " " << q.top()->freq << endl;
        q.pop();
    }
    std::cout << '\n';
}


void printTree(Node head) {
	if (head.Left) {
		printTree(*(head.Left));
		// printTree(*head.Right);
	}
	else {
		cout << head.s << ": " << head.freq << endl;
	}
}

void printCoding(map<string, string> coding) {
	map<string, string>::iterator itr;
	for (itr=coding.begin(); itr!=coding.end(); itr++){
		cout << "Char: " << itr->first << ", Encoding: " << itr->second << endl;
	}
}

void CodingFromTree(Node TreeTop, map<string, string> &coding, string prefix) {
	if (!TreeTop.Left) { // reached the bottom -- Null pointers for branches
		// cout << prefix << endl;
		coding.insert(pair<string, string> (TreeTop.s, prefix) );
	}
	else {
		// cout << "RECURSING: " << prefix + "0" << endl;
		// string prefix_left = prefix + "0";
		// cout << TreeTop.Left->freq << endl;
		CodingFromTree(*(TreeTop.Left), coding, prefix + "0");
		
		// cout << "RECURSING: " << prefix+"1" << endl;
		CodingFromTree(*TreeTop.Right, coding, prefix+"1");
	}
}

void CodingFromTree(Node TreeTop, map<string, string> &coding) {
	// called from the top of the tree
	CodingFromTree(TreeTop, coding, "");
}


Node generateTree(map<string, double> &coding) {
	// convert each character into a leaf
	map<string, double>::iterator itr;
	priority_queue<Node*, vector<Node*>, Comparator> tree;
	for (itr=coding.begin(); itr!= coding.end(); itr++) {
		Node* cur_leaf = new Node(itr->first, itr->second);
		tree.push(cur_leaf);
	}
	// cout << "All Leafs Created" << endl;
	// print_queue(tree);

	// build tree
	while (tree.size() > 1) {
		Node* left_node = tree.top();
		tree.pop();
		Node* right_node = tree.top();
		tree.pop();
		Node* newNode = new Node(left_node, right_node);
		tree.push(newNode);
	}
	return *(tree.top());
}


map<string, double> countCharacters(string s) {
	map<string, double> count;
	double l = s.size();
	string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*() .,:'[]{}-_=+";
	for (auto c: alphabet) {
		size_t n = std::count(s.begin(), s.end(), c);
		if (n > 0) {
			count.insert( pair<string, double> (string(1,c), (double)n));
		}
	}
	return count;
}

string compressString(string s, const map<string, string> &coding) {
	string compressed = "";
	for (auto c: s){
		compressed.append(coding.find(string(1,c))->second);
	}
	return compressed;
}

map<string, string> inverseCoding(map<string, string> coding) {
	map<string, string> icoding;
	map<string, string>::iterator itr;
	for (itr = coding.begin(); itr!= coding.end(); itr++) {
		icoding.insert( pair<string, string> (itr->second, itr->first));
	}
	return icoding;
}

string decode(string compressed_string, map<string, string> coding) {
	map<string, string> icoding = inverseCoding(coding);
	// printCoding(coding);
	// printCoding(icoding);
	string s = "";
	string code = "";
	map<string, string>::iterator itr;
	for (auto c: compressed_string) {
		code.append(string(1,c));
		itr = icoding.find(code);
		if (itr != icoding.end()) {
			s.append(itr->second);
			code = "";
		}
	}
	// cout << s << endl;
	return s;
}

class Compress {
public:
	string s;
	map<string, double> count;
	Node tree;
	map<string, string> coding;
	string compressed_string;
	Compress(string s_in) {
		s = s_in;
		count = countCharacters(s);
		tree = generateTree(count);
		CodingFromTree(tree, coding);
		compressed_string = compressString(s, coding);
		// cout << compressed_string << endl;
		// printCoding(coding);
	}

};


void Test1() {
	// count map
	map<string, double> count;
	// count.insert(pair<string, double> ("a", 1.0));
	// count.insert(pair<string, double> ("b", 2.0));
	// count.insert(pair<string, double> ("c", 3.0));
	// count.insert(pair<string, double> ("d", 6.0));
	// count.insert(pair<string, double> ("e", 12.0));
	// count.insert(pair<string, double> ("f", 24.0));

	// count.insert(pair<string, double> ("a", 1.0));
	// count.insert(pair<string, double> ("b", 1.0));
	// count.insert(pair<string, double> ("c", 1.0));
	// count.insert(pair<string, double> ("d", 1.0));
	// count.insert(pair<string, double> ("e", 1.0));
	// count.insert(pair<string, double> ("f", 1.0));

	Node tree = generateTree(count);	
	// printTree(tree);

	// generate coding
	map<string, string> coding;
	CodingFromTree(tree, coding);
	printCoding(coding);
}

void Test2() {
	string test_string = "Hello! My name is Ammar.";
	Compress C(test_string);
	cout << "Original String: " << test_string << endl;
	cout << "Compressed String: " << C.compressed_string << endl;
	cout << "Decoded String: " << decode(C.compressed_string, C.coding) << endl;
}

int main() {
	Test2();
	return 0;
}