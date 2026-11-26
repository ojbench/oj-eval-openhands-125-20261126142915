CXX = g++
CXXFLAGS = -O2 -std=c++17 -w

all: code

code: code.cpp
	$(CXX) $(CXXFLAGS) -o code code.cpp

clean:
	rm -f code
