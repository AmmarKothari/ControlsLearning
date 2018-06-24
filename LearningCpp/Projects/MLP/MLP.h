
#include <vector>
#include <string>
#include <fstream>

#define vec_float std::vector<float>
#define matrix_float std::vector< vec_float >
class MLP
{
public:
	MLP(int, int, int);
	// ~MLP();
	void printLayers();
	void train(int, int, std::vector <std::vector<int> > &, std::vector<int> &);
	// void train(int, int, std::vector <vec_float > &);
	float sigmoid(float);
	matrix_float sigmoid(matrix_float &);
	vec_float sigmoid(vec_float &);
	matrix_float transpose(matrix_float &);
	matrix_float transpose(vec_float &);
	matrix_float matmul(matrix_float &, matrix_float &);
	matrix_float matmul(float &, vec_float &);
	matrix_float matmul(float &, matrix_float &);
	matrix_float matmul(matrix_float &, vec_float &);
	matrix_float elementwiseMul(matrix_float &, matrix_float &);
	matrix_float elementwiseMul(vec_float &, matrix_float &);
	matrix_float elementwiseMul(matrix_float &, vec_float &);
	void matAdd(matrix_float &, matrix_float &, matrix_float &);
	matrix_float addDimension(vec_float &);
	vec_float removeDimension(matrix_float &);
	vec_float forward_1Layer(vec_float &, matrix_float &);
	vec_float forward_1Layer(std::vector<int> &, matrix_float &);
	std::vector<int> classify_LastLayer(vec_float &);
	float loss(int &, float &);
	vec_float add_bias_node(vec_float &);
	vec_float add_bias_node(std::vector<int> &);
	matrix_float removeBiasWeights(matrix_float &mat_in);
	void initializeMat(matrix_float &, int, int);
	void initializeMatRandom(matrix_float &, int , int );
	void printMatrix(const matrix_float &);
	void printMatrix(const vec_float &);
	void printMatrix(const std::vector<int> &);
	void openCSVFile(std::ofstream &, std::string);
	void closeCSVFile(std::ofstream &);

	int input_dims_ct;
	int hidden_nodes_ct;
	int bias_val;
	int output_dim_ct;
	float learning_rate;
	std::ofstream batch_recorder;
	

	matrix_float W1;
	matrix_float W2;

private:
};