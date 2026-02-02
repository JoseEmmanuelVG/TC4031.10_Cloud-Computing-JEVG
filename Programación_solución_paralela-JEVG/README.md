
# Programación de una solución paralela – Suma de arreglos con OpenMP

Este subproyecto corresponde a la **Tarea 1: Programación de una solución paralela**, cuyo objetivo es implementar la suma elemento a elemento de dos arreglos unidimensionales utilizando **programación paralela con OpenMP** en C++.

La solución permite ejecutar la suma en paralelo, analizar el impacto del número de hilos y registrar automáticamente los resultados de cada ejecución para su posterior análisis.

---

## 📌 Compilación y ejecución

### Usando Make (recomendado)
Desde la raíz del subproyecto `Programación_solución_paralela-JEVG`:

```bash
cd Programación_solución_paralela-JEVG
make clean   # limpia archivos generados
make         # compila el programa (usa g++ con OpenMP)
make run     # ejecuta el binario interactivo
```

### Compilación manual
También es posible compilar y ejecutar manualmente:

```bash
g++ -O2 -Wall -Wextra -fopenmp src/main-JEVG.cpp -o build/suma_arreglos
./build/suma_arreglos
```

---

## 🧭 Interacción con el programa (menú)

Al ejecutar el programa se muestra un menú interactivo con tres opciones para llenar los arreglos `a` y `b`:

### 1️⃣ Aleatorio
- Rellena los arreglos con números aleatorios.
- Permite introducir una semilla manual (vacío = semilla basada en tiempo).
- Permite elegir entre números enteros o flotantes.

### 2️⃣ Usuario
- Introducir manualmente los primeros `mostrar` valores de los arreglos `a` y `b`.
- El resto de los elementos se genera automáticamente para evitar tiempos largos de captura.

### 3️⃣ Cálculo
- Rellena los arreglos mediante un patrón determinista (caso base reproducible).

Tras cada ejecución, el programa pregunta si se desea realizar una nueva ejecución o finalizar.

---

## ⚙️ Configuración de hilos OpenMP

El número de hilos puede configurarse mediante la variable de entorno `OMP_NUM_THREADS`:

```bash
export OMP_NUM_THREADS=4
make run
```

Durante la ejecución, el programa reporta:

- Número máximo de hilos disponibles (`omp_get_max_threads`).
- Número de hilos utilizados dentro de la región paralela (`omp_get_num_threads`).

---

## 🗂️ Resultados y archivos de registro

Cada ejecución del programa genera automáticamente un archivo de registro en la carpeta:

```
docs/runs/
```

El nombre del archivo sigue el formato:

```
run_YYYYMMDD_HHMMSS.txt
```

### Contenido del archivo de resultados

Cada archivo de registro incluye la siguiente información:

- **Fecha y hora**: momento exacto de la ejecución.
- **Método de llenado**: opción seleccionada en el menú  
  - `1` = Aleatorio  
  - `2` = Usuario  
  - `3` = Cálculo  
- **Semilla**: valor de la semilla utilizada para generación aleatoria (`N/A` si no aplica).
- **Tipo aleatorio (`rand_kind`)**:  
  - `1` = Enteros  
  - `2` = Flotantes  
  - `0` = No aplica  
- **N**: tamaño total de los arreglos.
- **chunk**: tamaño del bloque asignado a cada hilo en la planificación estática.
- **mostrar**: cantidad de elementos impresos por arreglo.
- **hilos (`omp_get_max_threads`)**: número máximo de hilos disponibles.
- **hilos en región paralela (`omp_get_num_threads`)**: número real de hilos usados en la suma.
- **tiempo_paralelo**: tiempo de ejecución de la región paralela medido en segundos.
- **Primeros valores de los arreglos**:
  - Primeros `mostrar` valores del arreglo `a`.
  - Primeros `mostrar` valores del arreglo `b`.
  - Primeros `mostrar` valores del arreglo `c`, validando que `c[i] = a[i] + b[i]`.

Estos archivos permiten analizar y comparar ejecuciones sin necesidad de volver a ejecutar el programa, y sirven como evidencia del funcionamiento y desempeño de la solución paralela.

---

## 📝 Notas adicionales

- El código fuente se encuentra comentado de forma técnica y concisa para facilitar su comprensión.
- El proyecto está preparado para ejecutarse en Linux utilizando `g++` con soporte para OpenMP.
- No se utilizan dependencias externas.
