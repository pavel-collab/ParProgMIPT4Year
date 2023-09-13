all:
	g++ -fopenmp -o simple_schedule -DSCHEDULE=0 Balancing.cpp
	g++ -fopenmp -o static_schedule -DSCHEDULE=1 Balancing.cpp
	g++ -fopenmp -o static1_schedule -DSCHEDULE=2 Balancing.cpp
	g++ -fopenmp -o static4_schedule -DSCHEDULE=3 Balancing.cpp
	g++ -fopenmp -o dynamic1_schedule -DSCHEDULE=4 Balancing.cpp
	g++ -fopenmp -o dynamic4_schedule -DSCHEDULE=5 Balancing.cpp
	g++ -fopenmp -o guided1_schedule -DSCHEDULE=6 Balancing.cpp
	g++ -fopenmp -o guided4_schedule -DSCHEDULE=7 Balancing.cpp

simple:
	g++ -fopenmp -o simple_schedule -DSCHEDULE=0 Balancing.cpp
static:
	g++ -fopenmp -o static_schedule -DSCHEDULE=1 Balancing.cpp
static1:
	g++ -fopenmp -o static1_schedule -DSCHEDULE=2 Balancing.cpp
static4:
	g++ -fopenmp -o static4_schedule -DSCHEDULE=3 Balancing.cpp
dynamic1:
	g++ -fopenmp -o dynamic1_schedule -DSCHEDULE=4 Balancing.cpp
dynamic4:
	g++ -fopenmp -o dynamic4_schedule -DSCHEDULE=5 Balancing.cpp
guided1:
	g++ -fopenmp -o guided1_schedule -DSCHEDULE=6 Balancing.cpp
guided4:
	g++ -fopenmp -o guided4_schedule -DSCHEDULE=7 Balancing.cpp

clear:
	rm simple_schedule static_schedule static1_schedule static4_schedule dynamic1_schedule dynamic4_schedule guided1_schedule guided4_schedule