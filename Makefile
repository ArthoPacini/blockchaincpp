make:
	clang++ main.cpp -o main.exe -std=c++17 -pthread -O3 -g -lws2_32
#-D_GLIBCXX_DEBUG