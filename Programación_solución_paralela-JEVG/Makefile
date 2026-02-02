CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -fopenmp
SRC=src/main-JEVG.cpp
OUT=build/suma_arreglos

all: $(OUT)

build:
	mkdir -p build

$(OUT): build $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -rf build
