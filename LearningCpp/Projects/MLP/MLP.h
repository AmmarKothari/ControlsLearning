
#include <vector>



class MLP
{
public:
	MLP(int, int, int);
	// ~MLP();
	void printLayers();
	void train(int, int, std::vector <std::vector<int> > &);

	int input_dims_ct;
	int hidden_nodes_ct;
	int bias_val;
	int output_dim_ct;
	

	std::vector< std::vector<float> > W1;
	std::vector< std::vector<float> > W2;
	 // W1[hidden_nodes_ct][input_dims_ct+1];
	// float W2[hidden_nodes_ct+1][1];

private:
};