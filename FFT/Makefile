all:
	g++ -DTIME -DALGORITHM=0 -fopenmp -o naive_alg_time main.cpp
	g++ -DTIME -DALGORITHM=1 -fopenmp -o fft_time main.cpp
	g++ -DTIME -DALGORITHM=2 -fopenmp -o parallel_fft_time main.cpp

validation:
	g++ -DVALIDATION -DALGORITHM=0 -fopenmp -o naive_alg_validation main.cpp
	g++ -DVALIDATION -DALGORITHM=1 -fopenmp -o fft_validation main.cpp
	g++ -DVALIDATION -DALGORITHM=2 -fopenmp -o parallel_fft_validation main.cpp

clear:
	rm naive_alg_time fft_time parallel_fft_time
	rm naive_alg_validation fft_validation parallel_fft_validation
	rm *.dat

clear_signals:
	rm ./data/*.dat