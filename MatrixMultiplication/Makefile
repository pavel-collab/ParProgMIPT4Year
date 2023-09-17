all:
	g++ -fopenmp -DALGORITHM=0 -o simple_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=1 -o cache_friendly_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=2 -o parallel_simple_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=3 -o parallel_cache_friendly_matrix_multiplication MatrixMultiplication.cpp

clear:
	rm simple_matrix_multiplication cache_friendly_matrix_multiplication parallel_simple_matrix_multiplication parallel_cache_friendly_matrix_multiplication
	rm *.dat