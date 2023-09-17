all:
	g++ -fopenmp -DALGORITHM=0 -o simple_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=1 -o cache_friendly_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=2 -DSCHEDULE=0 -o parallel_simple_matrix_multiplication_static MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=2 -DSCHEDULE=1 -o parallel_simple_matrix_multiplication_dynamic MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=2 -DSCHEDULE=2 -o parallel_simple_matrix_multiplication_guided MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=3 -o parallel_cache_friendly_matrix_multiplication MatrixMultiplication.cpp
	g++ -o strassen StrassenAlgorithm.cpp
	
Validation:
	g++ -fopenmp -DALGORITHM=0 -DVALIDTION -o simple_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=1 -DVALIDTION -o cache_friendly_matrix_multiplication MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=2 -DVALIDTION -o parallel_simple_matrix_multiplication_static MatrixMultiplication.cpp
	g++ -fopenmp -DALGORITHM=3 -DVALIDTION -o parallel_cache_friendly_matrix_multiplication MatrixMultiplication.cpp
	g++ -DVALIDTION -o strassen StrassenAlgorithm.cpp

Strassen:
	g++ -o strassen StrassenAlgorithm.cpp

clear_dat:
	rm *.dat
	
clear_binary:
	rm simple_matrix_multiplication cache_friendly_matrix_multiplication parallel_simple_matrix_multiplication_static parallel_simple_matrix_multiplication_dynamic parallel_simple_matrix_multiplication_guided parallel_cache_friendly_matrix_multiplication strassen