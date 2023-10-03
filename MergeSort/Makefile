all:
	g++ -fopenmp -DVALIDTION -DMERGE -o merge_validation main.cpp
	g++ -fopenmp -DVALIDTION -DQUICK -o quick_validation main.cpp
	g++ -fopenmp -DVALIDTION -DBUBBLE -o bubble_validation main.cpp

	g++ -fopenmp -DTIME -DMERGE -o merge main.cpp
	g++ -fopenmp -DTIME -DQUICK -o quick main.cpp
	g++ -fopenmp -DTIME -DBUBBLE -o bubble main.cpp

Validation:
	g++ -fopenmp -DVALIDTION -DMERGE -o merge_validation main.cpp
	g++ -fopenmp -DVALIDTION -DQUICK -o quick_validation main.cpp
	g++ -fopenmp -DVALIDTION -DBUBBLE -o bubble_validation main.cpp

time:
	g++ -fopenmp -DTIME -DMERGE -o merge main.cpp
	g++ -fopenmp -DTIME -DQUICK -o quick main.cpp
	g++ -fopenmp -DTIME -DBUBBLE -o bubble main.cpp

clear_time:
	rm merge quick bubble

clear_validation: 
	rm merge_validation quick_validation bubble_validation

clear:
	rm merge quick bubble merge_validation quick_validation bubble_validation