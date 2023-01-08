all:
	g++ -fPIC -c re1.cpp -o d1.o
	g++ -shared d1.o -o libd1.so
	g++ -fPIC -c re2.cpp -o d2.o
	g++ -shared d2.o -o libd2.so

static1: prog1.cpp
	g++ prog1.cpp -L. -ld1 -o main1 -Wl,-rpath -Wl,.

static2: prog1.cpp
	g++ prog1.cpp -L. -ld2 -o main2 -Wl,-rpath -Wl,.

dynamic: prog2.cpp
	g++ prog2.cpp -L. -ldl -o main3 -Wl,-rpath -Wl,.

clean:
	rm -rf *.o *.so main1 main2 main3
