# sistemasOperativos-tarea2
Integrantes:
- Pablo Vargas
- Camila García
- Antonella Pincheira
- Benjamin Stuckrath

# Simulador de memoria virtual
Se incluye un simulador de traducción de direcciones de memoria virtual a real y proporciona información de fallos de pagina

Para compilar y ejecutar:
```
gcc -s sim.c -o sim
./sim Nmarcos tamañomarco [--verbose] traza.txt
[Ej: ./sim 8 4096 trace2.txt]
```
Donde Nmarcos es el número de marcos disponibles
tamañomarco el tamaño de cada marco en bytes
--verbose opcional, indica la secuencia paso a paso de la traducción
traza.txt archivo de texto que incluye por cada linea las dirreciones virtuales a procesar

# Sincronización con barrera
Implemtación de una barrera reutilizable que coordina múltiples hebras concurrentes.

Para compilar y ejecutar:
```
gcc barrier.c barrier_main.c -pthread
./a.out
```