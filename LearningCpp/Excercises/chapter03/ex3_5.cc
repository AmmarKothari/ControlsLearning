#include <ios>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector;
using std::istringstream;
using std::getline;	

int main() {
	cout << "Enter grades for multiple students.  "
			"Enter the students name seperated by spaces, followed by end of line. "
			"Then enter all of the grades seperated by spaces, followed by end of line. "
			"Assumed that there are 5 grades per student."
			<< endl;

	vector<string> all_students;
	vector<double> avg_grades;

	string line;
	getline(cin, line);
	istringstream iss(line);
	string x_names;
	while (iss >> x_names) {
		cout << "Name: " << x_names << endl;
		all_students.push_back(x_names);
	}

	int x_grades;
	int count = 0;
	int sum = 0;
	while (cin >> x_grades) {
		++count;
		sum += x_grades;
		if (count == 4) {
			avg_grades.push_back(sum/count);
			sum = 0;
			count = 0;
		}
	}

	cout << endl;

	for (int i = 0; i!=all_students.size(); ++i) {
		cout << "Student: " << all_students[i] << "\t";
		cout << "Average Grade: " << avg_grades[i];
		cout << endl;
	}

	return 0; // success
}