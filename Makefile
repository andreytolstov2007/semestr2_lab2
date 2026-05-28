main: main.o test_modul.o
	g++ main.o test_modul.o -o a

main.o: main.cpp
	g++ main.cpp -c

test_modul.o: test_modul.cpp
	g++ test_modul.cpp -c