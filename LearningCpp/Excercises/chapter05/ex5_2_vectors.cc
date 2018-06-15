#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ios>
#include <iomanip>
#include <stdexcept>

#include "Student_info_vectors.h"
#include "median_vectors.h"
#include "grade_vectors.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::max;
using std::streamsize;
using std::setprecision;
using std::domain_error;

int main() {
  cout << "Enter each students name followed by their scores seperated only by a space." << endl;
  vector<Student_info> students;
  Student_info record;
  string::size_type maxlen = 0;

  // read and store all the records and find the length of the longest name
  while (read(cin, record)) {
    maxlen = max(maxlen, record.name.size());
    students.push_back(record);
  }

  // alphabetize the records
  sort(students.begin(), students.end(), compare);
  for (vector<Student_info>::size_type i = 0; i != students.size(); ++i) {
    // write the name. padded on the right to maxlen + 1 characters
    cout << students[i].name << string(maxlen + 1 - students[i].name.size(), ' ');

    // compute and write the grade
    try {
      double final_grade = grade(students[i]);
      streamsize prec = cout.precision();
      cout << setprecision(3) << final_grade << setprecision(prec);
    }
    catch (domain_error e) {
      cout << e.what();
    }
    cout << endl;
  }
  return 0;
}
