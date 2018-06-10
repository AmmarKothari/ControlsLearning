


#include <vector>

#include "Student_info_vectors.h"

using std::istream;
using std::vector;

bool compare(const Student_info& x, const Student_info& y) {
  // returns if the first entry is alphabetically before the second entry
  return x.name < y.name;
}

istream& read(istream& is, Student_info& s)
{
  is >> s.name >> s.midterm >> s.final;
  read_hw(is, s.homework);
  return is;
}

istream& read_hw(istream& in, vector<double>& hw)
{
  if (in) {
    // get rid of previous contents
    hw.clear();

    // read homework grades
    double x;
    while (in >> x){
      hw.push_back(x);
    }

    // clear the stream so that input will work for next student
    in.clear();
  }
  return in;
}
