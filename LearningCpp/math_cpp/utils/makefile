CXX = g++ -v
CXXFLAGS = -std=c++11 -Wall


test: test_gen_noise_data
	./test_gen_noise_data

gen_noise_data.o: gen_noise_data.cpp vector_ops.o
	$(CXX) $(CXXFLAGS) gen_noise_data.cpp -c -o gen_noise_data.o

vector_ops.o: vector_ops.cpp
	$(CXX) $(CXXFLAGS) vector_ops.cpp -c -o vector_ops.o

test_gen_noise_data: gen_noise_data.o test_gen_noise_data.cpp
	$(CXX) $(CXXFLAGS) test_gen_noise_data.cpp -o test_gen_noise_data

test_write_1d_noisy_data: gen_noise_data.o write_noise_data.cpp
	rm one_d_noisy.csv
	$(CXX) $(CXXFLAGS) write_noise_data.cpp -o write_noise_data
	./write_noise_data
	python plot_noisy_data.py --one_d

test_write_sin_noisy_data: gen_noise_data.o write_noise_data.cpp
	rm sin_noisy_data.csv
	$(CXX) $(CXXFLAGS) write_noise_data.cpp -o write_noise_data
	./write_noise_data
	python plot_noisy_data.py --sin_data

clobber:
	rm -f gen_noise_data test_gen_noise_data