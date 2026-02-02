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
// Registro de ejecución: devuelve la ruta del fichero creado (vacío si falla).
std::string log_run(int metodo, long long semilla, int rand_kind, int max_threads, int num_threads_region, double tiempo, float *a, float *b, float *c);

// Leer respuesta sí/no con validación
static bool read_yesno(const std::string &prompt) {
    while (true) {
        std::string line;
        std::cout << prompt;
        if (!std::getline(std::cin, line)) return false;
        if (line.empty()) continue;
        char c = line[0];
        if (c == 's' || c == 'S' || c == 'y' || c == 'Y') return true;
        if (c == 'n' || c == 'N') return false;
        std::cout << "Respuesta inválida. Responde 's' o 'n'.\n";
    }
}

// Leer entero con validación (prompt, rango inclusive)
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

// Leer float con validación (prompt)
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

    // Bucle principal para permitir múltiples ejecuciones
    do {
        std::cout << "Elige método para llenar a y b:\n";
        std::cout << " 1) Aleatorio (seed configurable)\n";
        std::cout << " 2) Entrada por usuario (solo primeros " << mostrar << ")\n";
        std::cout << " 3) Cálculo (por defecto)\n";
        int opt = read_int("Opción [1-3]: ", 1, 3);

        // Semilla global para opciones aleatorias (-1 = no aplicable)
        long long seed = -1;
        // rand_kind: 0 = N/A, 1 = enteros, 2 = floats
        int rand_kind = 0;

        if (opt == 1) {
            // Opción aleatoria: solicitar semilla (vacío -> basada en tiempo)
            std::cout << "Introduce semilla (vacío = time-based): ";
            std::string s; std::getline(std::cin, s);
            if (s.empty()) seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            else {
                std::stringstream ss(s);
                if (!(ss >> seed)) seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            }

            // Inicializar generador con la semilla global
            std::mt19937_64 rng(static_cast<unsigned long long>(seed));
            std::cout << "Generar enteros (1) o floats (2)? ";
            int kind = read_int("[1-2]: ", 1, 2);
            rand_kind = kind;
            if (kind == 1) {
                // Distribución uniforme de enteros
                std::uniform_int_distribution<int> dist(-1000, 1000);
                for (i = 0; i < N; ++i) {
                    a[i] = static_cast<float>(dist(rng));
                    b[i] = static_cast<float>(dist(rng));
                }
            } else {
                // Distribución uniforme de reales (float)
                std::uniform_real_distribution<float> dist(-1000.0f, 1000.0f);
                for (i = 0; i < N; ++i) {
                    a[i] = dist(rng);
                    b[i] = dist(rng);
                }
            }

        } else if (opt == 2) {
            // Entrada del usuario para los primeros 'mostrar' valores; el resto se genera automáticamente
            std::cout << "Introduce hasta " << mostrar << " valores para 'a' (uno por línea):\n";
            for (i = 0; i < mostrar; ++i) {
                a[i] = read_float("a[" + std::to_string(i) + "] = ");
            }
            std::cout << "Introduce hasta " << mostrar << " valores para 'b' (uno por línea):\n";
            for (i = 0; i < mostrar; ++i) {
                b[i] = read_float("b[" + std::to_string(i) + "] = ");
            }
            // Rellenar el resto automáticamente
            for (i = mostrar; i < N; ++i) {
                a[i] = i * 10.0f;
                b[i] = (i + 3) * 3.7f;
            }

        } else {
            // Relleno por cálculo (comportamiento original)
            for (i = 0; i < N; i++) {
                a[i] = i * 10;
                b[i] = (i + 3) * 3.7f;
            }
        }

        int pedazos = chunk;

        // Obtener número de hilos máximos y el número de hilos dentro de una región paralela
        int max_threads = omp_get_max_threads();
        int num_threads_region = 0;
        #pragma omp parallel
        {
            #pragma omp single
            num_threads_region = omp_get_num_threads();
        }

        // Medir tiempo y ejecutar suma en paralelo (región de trabajo)
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

        // Registrar ejecución en fichero y mostrar la ruta en consola
        std::string logfile = log_run(opt, seed, rand_kind, max_threads, num_threads_region, (t1 - t0), a, b, c);
        if (!logfile.empty()) std::cout << "Registro guardado en: " << logfile << "\n";

    } while (read_yesno("¿Desea realizar otra ejecución? (s/n): "));

    std::cout << "Finalizando. Gracias.\n";
    return 0;
}

void imprimeArreglo(float *d) {
    for (int x = 0; x < mostrar; x++) {
        std::cout << d[x] << " - ";
    }
    std::cout << std::endl;
}

// Registrar la ejecución en un fichero timestamped dentro de docs/runs/
// Devuelve la ruta del fichero creado o cadena vacía si hubo error.
std::string log_run(int metodo, long long semilla, int rand_kind, int max_threads, int num_threads_region, double tiempo, float *a, float *b, float *c) {
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
    if (!ofs) return std::string();

    ofs << "Fecha y hora: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '\n';
    ofs << "Método llenado: " << metodo << "\n";
    if (semilla >= 0) ofs << "Semilla: " << semilla << "\n";
    else ofs << "Semilla: N/A\n";
    ofs << "Tipo aleatorio (rand_kind): " << rand_kind << " (1=enteros,2=floats,0=N/A)\n";
    ofs << "N: " << N << "\n";
    ofs << "chunk: " << chunk << "\n";
    ofs << "mostrar: " << mostrar << "\n";
    ofs << "hilos (omp_get_max_threads): " << max_threads << "\n";
    ofs << "hilos en región paralela (omp_get_num_threads): " << num_threads_region << "\n";
    ofs << std::fixed << std::setprecision(6);
    ofs << "tiempo_paralelo: " << tiempo << " segundos\n\n";

    ofs << "Primeros " << mostrar << " valores de a:\n";
    for (int i = 0; i < mostrar; ++i) ofs << a[i] << (i+1<mostrar?", ":"\n");
    ofs << "Primeros " << mostrar << " valores de b:\n";
    for (int i = 0; i < mostrar; ++i) ofs << b[i] << (i+1<mostrar?", ":"\n");
    ofs << "Primeros " << mostrar << " valores de c:\n";
    for (int i = 0; i < mostrar; ++i) ofs << c[i] << (i+1<mostrar?", ":"\n");

    ofs.close();
    return fname.str();
}
