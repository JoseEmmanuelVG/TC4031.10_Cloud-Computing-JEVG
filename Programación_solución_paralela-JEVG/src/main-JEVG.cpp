#include <iostream>
#include <omp.h>
#include <random>
#include <string>
#include <sstream>
#include <limits>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <ctime>

#define N 1000
#define chunk 100
#define mostrar 10

void imprimeArreglo(float *d);
void log_run(int metodo, long long semilla, double tiempo, float *a, float *b, float *c);

// Read integer option with validation
static int read_int(const std::string &prompt, int minv, int maxv) {
    while (true) {
        std::string line;
        std::cout << prompt;
        if (!std::getline(std::cin, line)) return minv;
        std::stringstream ss(line);
        int v; char c;
        if (ss >> v && !(ss >> c)) {
            if (v >= minv && v <= maxv) return v;
        }
        std::cout << "Entrada inválida. Intenta de nuevo.\n";
    }
}

// Read float with validation
static float read_float(const std::string &prompt) {
    while (true) {
        std::string line;
        std::cout << prompt;
        if (!std::getline(std::cin, line)) return 0.0f;
        std::stringstream ss(line);
        float v; char c;
        if (ss >> v && !(ss >> c)) return v;
        std::cout << "Entrada inválida. Ingresa un número.\n";
    }
}

int main() {
    std::cout << "Sumando Arreglos en Paralelo! - JEVG\n";

    static float a[N], b[N], c[N];
    int i;

    std::cout << "Elige método para llenar a y b:\n";
    std::cout << " 1) Aleatorio (seed configurable)\n";
    std::cout << " 2) Entrada por usuario (solo primeros " << mostrar << ")\n";
    std::cout << " 3) Cálculo (por defecto)\n";
    int opt = read_int("Opción [1-3]: ", 1, 3);

    long long seed = -1; // -1 means not applicable

    if (opt == 1) {
        // Random fill
        long long seed = 0;
        std::cout << "Introduce semilla (vacío = time-based): ";
        std::string s; std::getline(std::cin, s);
        if (s.empty()) seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        else {
            std::stringstream ss(s);
            if (!(ss >> seed)) seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        }

        std::mt19937_64 rng(static_cast<unsigned long long>(seed));
        std::cout << "Generar enteros (1) o floats (2)? ";
        int kind = read_int("[1-2]: ", 1, 2);
        if (kind == 1) {
            std::uniform_int_distribution<int> dist(-1000, 1000);
            for (i = 0; i < N; ++i) {
                a[i] = static_cast<float>(dist(rng));
                b[i] = static_cast<float>(dist(rng));
            }
        } else {
            std::uniform_real_distribution<float> dist(-1000.0f, 1000.0f);
            for (i = 0; i < N; ++i) {
                a[i] = dist(rng);
                b[i] = dist(rng);
            }
        }

    } else if (opt == 2) {
        // User input for first 'mostrar' values; rest generated automatically
        std::cout << "Introduce hasta " << mostrar << " valores para 'a' (uno por línea):\n";
        for (i = 0; i < mostrar; ++i) {
            a[i] = read_float("a[" + std::to_string(i) + "] = ");
        }
        std::cout << "Introduce hasta " << mostrar << " valores para 'b' (uno por línea):\n";
        for (i = 0; i < mostrar; ++i) {
            b[i] = read_float("b[" + std::to_string(i) + "] = ");
        }
        // Fill remainder automatically (fast pattern)
        for (i = mostrar; i < N; ++i) {
            a[i] = i * 10.0f;
            b[i] = (i + 3) * 3.7f;
        }

    } else {
        // Calculation fill (original behavior)
        for (i = 0; i < N; i++) {
            a[i] = i * 10;
            b[i] = (i + 3) * 3.7f;
        }
    }

    int pedazos = chunk;

    double t0 = omp_get_wtime();

    #pragma omp parallel for shared(a,b,c,pedazos) private(i) schedule(static, pedazos)
    for (i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }

    double t1 = omp_get_wtime();

    std::cout << "Tiempo (paralelo) = " << std::fixed << std::setprecision(6) << (t1 - t0) << " segundos\n\n";

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo a:\n";
    imprimeArreglo(a);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo b:\n";
    imprimeArreglo(b);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo c:\n";
    imprimeArreglo(c);

    // Log run to file (also keeps console output)
    log_run(opt, seed, (t1 - t0), a, b, c);

    return 0;
}

void imprimeArreglo(float *d) {
    for (int x = 0; x < mostrar; x++) {
        std::cout << d[x] << " - ";
    }
    std::cout << std::endl;
}

// Log the run to a timestamped file under docs/runs/
void log_run(int metodo, long long semilla, double tiempo, float *a, float *b, float *c) {
    namespace fs = std::filesystem;
    try {
        fs::create_directories("docs/runs");
    } catch (...) {}

    // timestamp for filename
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream fname;
    fname << "docs/runs/run_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt";

    std::ofstream ofs(fname.str());
    if (!ofs) return;

    ofs << "Fecha y hora: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '\n';
    ofs << "Método llenado: " << metodo << "\n";
    if (semilla >= 0) ofs << "Semilla: " << semilla << "\n";
    else ofs << "Semilla: N/A\n";
    ofs << "N: " << N << "\n";
    ofs << "chunk: " << chunk << "\n";
    ofs << "mostrar: " << mostrar << "\n";
    ofs << "hilos (omp_get_max_threads): " << omp_get_max_threads() << "\n";
    ofs << std::fixed << std::setprecision(6);
    ofs << "tiempo_paralelo: " << tiempo << " segundos\n\n";

    ofs << "Primeros " << mostrar << " valores de a:\n";
    for (int i = 0; i < mostrar; ++i) ofs << a[i] << (i+1<mostrar?", ":"\n");
    ofs << "Primeros " << mostrar << " valores de b:\n";
    for (int i = 0; i < mostrar; ++i) ofs << b[i] << (i+1<mostrar?", ":"\n");
    ofs << "Primeros " << mostrar << " valores de c:\n";
    for (int i = 0; i < mostrar; ++i) ofs << c[i] << (i+1<mostrar?", ":"\n");

    ofs.close();
}
