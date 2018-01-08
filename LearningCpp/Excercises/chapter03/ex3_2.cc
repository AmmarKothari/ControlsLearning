#include <algorithm>
#include <iomanip>
#ifndef __GNUC__
#include <ios>
#endif
#include <iostream>
#include <string>
#include <vector>

using std::cin;             using std::sort;
using std::cout;            using std::streamsize;
using std::endl;            using std::string;
using std::setprecision;    using std::vector;

int main()
{
	// ask for the homework grades
	cout << "Enter all your homework grades, "
	        "followed by end-of-file: ";

	// the number and sum of grades read so far
	int count = 0;
	double sum = 0;

	// a variable into which to read
	double x;

	vector<double> homework;
	// invariant: `homework' contains all the homework grades read so far
	// cout << "Here?" << endl;
	while (cin >> x)
		homework.push_back(x);
	cout << endl;

	// check that the student entered some homework grades
#ifdef _MSC_VER
	typedef std::vector<double>::size_type vec_sz;
#else
	typedef vector<double>::size_type vec_sz;
#endif
	vec_sz size = homework.size();
	if (size == 0) {
		cout << endl << "You must enter your grades.  "
		                "Please try again." << endl;	
		return 1;
	}

	// sort the grades
	sort(homework.begin(), homework.end());

	// compute the median homework grade
	vec_sz mid = size/2;
	double median;
	median = size % 2 == 0 ? (homework[mid] + homework[mid-1]) / 2
	                       : homework[mid];

   vector<vec_sz> quartiles_ind;
   quartiles_ind.push_back(0);
   quartiles_ind.push_back(size/4);
   quartiles_ind.push_back(size/2);
   quartiles_ind.push_back(size - size/4);
   quartiles_ind.push_back(size);

   streamsize prec = cout.precision();
   cout << setprecision(3);

   vector<string> stringQuartiles;
   stringQuartiles.push_back("Lowest");
   stringQuartiles.push_back("Low");
   stringQuartiles.push_back("Upper");
   stringQuartiles.push_back("Highest");
   
   for (int s = 0; s != stringQuartiles.size(); ++s) {
   		cout << stringQuartiles[s] << " Quartile: ";
	   	cout <<  quartiles_ind[s];
	   	cout << endl;
	   	for (int i = quartiles_ind[s]; i != quartiles_ind[s+1]; ++i) {
	   			cout << homework[i] << " ";
	   	}
   		cout << endl;
   }
   
   cout << setprecision(prec);

	return 0;
}

