#include <vector>
#include <stdexcept>
#include <string>

using namespace std;

#define vec_2d_float vector<vec_float>
#define vec_float vector<float>

// class vec_ops_exception: public runtime_error
// {
//   virtual const char* what() const throw()
//   {
//   	string message = "Vec Ops Exception: ";
//   	message +=runtime_error::what();	
//     return extra_message + passed_in_message;
//   }
// } vec_ops_exception;


vec_2d_float zip(vec_float v1, vec_float v2){
	if (v1.size() != v2.size()){
		throw std::runtime_error("Vectors not the same length.");
	}
	vec_2d_float zipped_vec;
	vec_float row_vec;
	for (int i=0; i++; i<=v1.size()){
		row_vec.push_back(v1[i]);
		row_vec.push_back(v2[i]);
		zipped_vec.push_back(row_vec);
		row_vec.clear();
	}
	return zipped_vec;
}
