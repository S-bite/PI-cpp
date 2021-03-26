pi: main.cpp BigInt.cpp BigFloat.cpp FFT-fast.cpp testUtil.cpp
	g++ main.cpp BigInt.cpp BigFloat.cpp FFT-fast.cpp testUtil.cpp -std=c++17 -g -pthread