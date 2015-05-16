all:
	g++ -std=c++11 bal.cpp -o bal
	g++ -std=c++11 bal.cpp -o bal.exe

clean:
	rm -rf *o bal bal.exe
