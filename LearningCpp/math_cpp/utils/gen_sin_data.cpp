#include <vector>

using namespace std;

#define vec_float vector<float>
#define vec_2d_float vector< vec_float>

vec_2d_float get_sin_data(float max_time, float time_step){
	vec_2d_float xy_data;
	vec_float xy;
	float val;
	while(val < max_time){
		xy.push_back(val);
		xy.push_back(sin(val));
		xy_data.push_back(xy);
		xy.clear();
		val += time_step;
	};
	return xy_data;
};