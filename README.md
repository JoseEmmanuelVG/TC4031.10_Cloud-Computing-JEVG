# TC4031.10 – Cloud Computing  
## Repositorio de prácticas – ITESM

Este repositorio concentra las **prácticas y tareas desarrolladas** para la materia **TC4031.10 – Cloud Computing** del Tecnológico de Monterrey (ITESM).  
El objetivo principal es **documentar, versionar y evidenciar** la implementación de soluciones relacionadas con cómputo en la nube.

Cada carpeta corresponde a una tarea específica e incluye:
- Código fuente.
- Scripts de compilación o ejecución.
- Evidencias de ejecución.
- Documentación de apoyo para su comprensión y validación.

---

## 📂 Índice de tareas

### 🔹 Tarea 1. Programación de una solución paralela
**Tema:** Paralelismo y programación paralela con OpenMP  
**Lenguaje:** C++  
**Modelo de paralelización:** OpenMP (paralelismo a nivel de datos)

📁 **Ubicación:**  
`Programación_solución_paralela-JEVG/`  

🔗 **Acceso directo:**  
https://github.com/JoseEmmanuelVG/TC4031.10_Cloud-Computing-JEVG/tree/main/Programaci%C3%B3n_soluci%C3%B3n_paralela-JEVG

### 🧩 Descripción general – Tarea 1

En esta tarea se desarrolla una **solución paralela** para realizar la suma elemento a elemento de dos arreglos unidimensionales, almacenando el resultado en un tercer arreglo. La implementación hace uso de **OpenMP** para paralelizar el ciclo principal, permitiendo distribuir el trabajo entre múltiples hilos y analizar el impacto del paralelismo en el tiempo de ejecución.

La solución incluye:
- Definición de constantes de precompilación para controlar tamaño de datos y granularidad.
- Menú interactivo para seleccionar el método de llenado de los arreglos:
  - Generación aleatoria (con semilla configurable).
  - Captura manual de datos por parte del usuario.
  - Generación basada en un cálculo determinista.
- Uso de un `for` paralelo con planificación estática (`schedule(static, chunk)`).
- Medición del tiempo de ejecución paralelo.
- Registro automático de cada ejecución en archivos de bitácora.
- Impresión parcial de resultados para validación.
