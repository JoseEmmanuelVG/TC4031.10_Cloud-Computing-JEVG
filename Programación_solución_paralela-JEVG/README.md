Proyecto de ejemplo: suma de arreglos en paralelo con OpenMP.

Compilación y ejecución
- Usando Make (en la raíz del subproyecto `Programación_solución_paralela-JEVG`):

```bash
cd Programación_solución_paralela-JEVG
make clean  # 
make        # compila (usa g++ -fopenmp)
make run    # ejecuta el binario interactivo
```

También puede compilar manualmente:

```bash
g++ -O2 -Wall -Wextra -fopenmp src/main-JEVG.cpp -o build/suma_arreglos_j
./build/suma_arreglos_j
```

Interacción (menú)
- Al ejecutar el programa se muestra un menú con tres opciones:
	1) Aleatorio: rellena `a` y `b` con números aleatorios. Puedes introducir una semilla (vacío = semilla basada en tiempo) y elegir enteros o floats.
	2) Usuario: introducir manualmente hasta los primeros `mostrar` valores para `a` y `b`; el resto se genera automáticamente para no tardar.
	3) Cálculo: relleno por patrón (comportamiento original del ejemplo).

Hilos OpenMP
- Puedes fijar el número de hilos con la variable de entorno `OMP_NUM_THREADS`:

```bash
export OMP_NUM_THREADS=4
make run
```

Logs de ejecución
- Cada ejecución crea un archivo en `docs/runs/` con nombre `run_YYYYMMDD_HHMMSS.txt`.
- El log contiene: fecha/hora, método elegido, semilla (si aplica), tipo aleatorio (enteros/flotantes), valores de `N`, `chunk`, `mostrar`, número de hilos (`omp_get_max_threads`), número de hilos detectados dentro de una región paralela (`omp_get_num_threads`), tiempo paralelo y los primeros `mostrar` valores de `a`, `b` y `c`.

Notas
- El código está preparado para compilar en Linux con `g++ -fopenmp` y no usa dependencias externas.
