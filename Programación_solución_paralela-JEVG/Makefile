
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -fopenmp
SRC=src/main-JEVG.cpp
OUT=build/suma_arreglos_j

# Default target: build the binary
all: $(OUT)

# Ensure build directory exists
build:
	mkdir -p build

# Link object (rebuild only when source changes)
$(OUT): build $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

# Run without forcing rebuild (depends on $(OUT) so it will build if missing)
run: $(OUT)
	./$(OUT)

# Clean build artifacts
clean:
	rm -rf build

