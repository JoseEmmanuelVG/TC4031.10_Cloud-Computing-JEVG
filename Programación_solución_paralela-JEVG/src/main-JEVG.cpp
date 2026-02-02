#include <iostream>
#include <omp.h>

#define N 1000
#define chunk 100
#define mostrar 10

void imprimeArreglo(float *d);

int main() {
    std::cout << "Sumando Arreglos en Paralelo! - JEVG\n";

    float a[N], b[N], c[N];
    int i;

    // Llenado de arreglos (basado en cálculo simple)
    for (i = 0; i < N; i++) {
        a[i] = i * 10;
        b[i] = (i + 3) * 3.7f;
    }

    int pedazos = chunk;

    double t0 = omp_get_wtime();

    #pragma omp parallel for shared(a,b,c,pedazos) private(i) schedule(static, pedazos)
    for (i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }

    double t1 = omp_get_wtime();

    std::cout << "Tiempo (paralelo) = " << (t1 - t0) << " segundos\n\n";

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo a:\n";
    imprimeArreglo(a);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo b:\n";
    imprimeArreglo(b);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo c:\n";
    imprimeArreglo(c);

    return 0;
}

void imprimeArreglo(float *d) {
    for (int x = 0; x < mostrar; x++) {
        std::cout << d[x] << " - ";
    }
    std::cout << std::endl;
}
