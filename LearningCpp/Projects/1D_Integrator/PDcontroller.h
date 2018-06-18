
#include <vector>


class PDcontroller
{
public:
	std::vector<float> xdes = {0,0};
	std::vector<float> xcur = {0,0};
	std::vector<float> e = {10,0};
	std::vector<float> e_last = {0,0};
	float Kp = 1000;
	float Kd = 10;


	// PDcontroller();
	// ~PDcontroller();

	float getControlInput(std::vector<float>, std::vector<float>, float);

private:
};