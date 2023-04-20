default:
	g++ main.cpp -o main
	./main.exe
for_others:
	g++ main.cpp -o main -static-libstdc++ -static -L-libunwind