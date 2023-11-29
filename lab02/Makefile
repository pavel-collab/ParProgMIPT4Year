task2_validation:
	g++ -g -fopenmp -DVALIDATION -DINNER -o task2_inner task2.cpp
	g++ -g -fopenmp -DVALIDATION -DOUTTER -o task2_outter task2.cpp
	g++ -g -fopenmp -DVALIDATION -DBOTH -o task2_both task2.cpp

task2_time:
	g++ -g -fopenmp -DTIME -DSEQ -o task2_seq task2.cpp
	g++ -g -fopenmp -DTIME -DINNER -o task2_inner task2.cpp
	g++ -g -fopenmp -DTIME -DOUTTER -o task2_outter task2.cpp
	g++ -g -fopenmp -DTIME -DBOTH -o task2_both task2.cpp

task1_validation:
	mpic++ task1.cpp -o task1_parallel
	g++ task1_cosistent.cpp -o task1_consistent

task1_time:
	mpic++ task1.cpp -o task1_parallel
	g++ task1_cosistent.cpp -o task1_consistent