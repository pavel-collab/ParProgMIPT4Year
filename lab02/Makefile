task1_validation:
	mpic++ task1.cpp -DVALIDATION -o task1_parallel
	g++ task1_cosistent.cpp -DVALIDATION -o task1_consistent

task1_time:
	mpic++ task1.cpp -DTIME -o task1_parallel
	g++ task1_cosistent.cpp -DTIME -o task1_consistent

task2_validation:
	g++ -g -fopenmp -DVALIDATION -DINNER -o task2_inner task2.cpp
	g++ -g -fopenmp -DVALIDATION -DOUTTER -o task2_outter task2.cpp
	g++ -g -fopenmp -DVALIDATION -DBOTH -o task2_both task2.cpp

task2_time:
	g++ -g -fopenmp -DTIME -DSEQ -o task2_seq task2.cpp
	g++ -g -fopenmp -DTIME -DINNER -o task2_inner task2.cpp
	g++ -g -fopenmp -DTIME -DOUTTER -o task2_outter task2.cpp
	g++ -g -fopenmp -DTIME -DBOTH -o task2_both task2.cpp

task3_validation:
	g++ -g -fopenmp -DVALIDATION -o task3_validation task3.cpp

task3_time:
	g++ -g -fopenmp -DTIME -DSEQ -o task3_seq task3.cpp
	g++ -g -fopenmp -DTIME -DINNER -o task3_inner task3.cpp