
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -fopenmp
SRC=src/main-JEVG.cpp
OUT=build/suma_arreglos_j

# Objetivo predeterminado: compilar el binario
all: $(OUT)

# Asegúrate de que el directorio de compilación existe.
build:
	mkdir -p build

# Objeto de enlace (reconstruir solo cuando cambie el origen)
$(OUT): build $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

# Ejecutar sin forzar recompilación (depende de $(OUT) para construir si falta)
run: $(OUT)
	./$(OUT)

# Limpiar artefactos de compilación
clean:
	rm -rf build

