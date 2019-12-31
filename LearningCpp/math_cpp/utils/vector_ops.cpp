#include <stdexcept>
#include <string>

#include "type_defines.h"

using namespace std;

// class vec_ops_exception: public runtime_error
// {
//   virtual const char* what() const throw()
//   {
//       string message = "Vec Ops Exception: ";
//       message +=runtime_error::what();
//     return extra_message + passed_in_message;
//   }
// } vec_ops_exception;

vec_2d_float make_2d(vec_float &v1)
{
	vec_2d_float vec_2d;
	vec_float row_vec;
	for (auto &v1_row : v1)
	{
		row_vec.push_back(v1_row);
		vec_2d.push_back(row_vec);
		row_vec.clear();
	}
	return vec_2d;
}

vec_2d_float zip(vec_2d_float &v1, vec_2d_float &v2)
{
	if (v1.size() != v2.size())
	{
		throw std::runtime_error("Vectors not the same length.");
	}
	vec_2d_float zipped_vec;
	vec_float row_vec;
	vec_float row_v1;
	vec_float row_v2;
	for (int i = 0; i < v1.size(); i++)
	{
		row_v1 = v1[i];
		row_vec.insert(row_vec.end(), row_v1.begin(), row_v1.end());
		row_v2 = v2[i];
		row_vec.insert(row_vec.end(), row_v2.begin(), row_v2.end());
		zipped_vec.push_back(row_vec);
		row_vec.clear();
	}
	return zipped_vec;
}

vec_2d_float zip(vec_float &v1, vec_2d_float &v2)
{
	vec_2d_float v1_expand = make_2d(v1);
	vec_2d_float zipped = zip(v1_expand, v2);
	return zipped;
}

vec_2d_float zip(vec_2d_float &v1, vec_float &v2)
{
	vec_2d_float v2_expand = make_2d(v2);
	vec_2d_float zipped = zip(v1, v2_expand);
	return zipped;
}