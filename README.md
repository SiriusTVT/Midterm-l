# Multi-Level Feedback Queue (MLFQ) Scheduler

Implementación en C++ de un simulador de planificación Multi-Level Feedback Queue usando programación orientada a objetos.

## Descripción

Este proyecto implementa un algoritmo de planificación MLFQ que permite simular diferentes configuraciones de colas con distintos algoritmos de planificación:

- **Round Robin (RR)** con quantum configurable
- **Shortest Job First (SJF)**
- **Shortest Time to Completion First (STCF)**

## Estructura del Proyecto

```
├── main.cpp              # Programa principal
├── Proceso.h/.cpp        # Clase Proceso
├── Cola.h/.cpp           # Clase Cola (maneja algoritmos)
├── PlanificadorMLFQ.h/.cpp # Clase principal del planificador
├── ArchivoUtils.h/.cpp   # Utilidades para manejo de archivos
├── Makefile              # Archivo de compilación
├── README.md             # Este archivo
└── Pruebas/              # Archivos de prueba
    ├── mlq001.txt
    ├── mlq002.txt
    └── ...
```

## Compilación

### Usando Makefile (recomendado)
```bash
make
```

### Compilación manual
```bash
g++ -std=c++11 -Wall -Wextra -O2 -o mlfq_scheduler main.cpp Proceso.cpp Cola.cpp PlanificadorMLFQ.cpp ArchivoUtils.cpp
```

### Para Windows
```bash
make windows
```

## Uso

### Ejecución básica
```bash
./mlfq_scheduler
```

El programa pedirá la ruta del archivo de entrada y mostrará un menú con opciones.

### Formato de archivo de entrada
```
# Comentarios comienzan con #
# Formato: etiqueta;burstTime;arrivalTime;queueLevel;priority
A; 6; 0; 1; 5
B; 9; 0; 1; 4
C; 10; 0; 2; 3
```

## Configuraciones Predefinidas

### Configuración A: RR(1), RR(3), RR(4), SJF
- Cola 1: Round Robin con quantum 1
- Cola 2: Round Robin con quantum 3
- Cola 3: Round Robin con quantum 4
- Cola 4: Shortest Job First

### Configuración B: RR(2), RR(3), RR(4), STCF
- Cola 1: Round Robin con quantum 2
- Cola 2: Round Robin con quantum 3
- Cola 3: Round Robin con quantum 4
- Cola 4: Shortest Time to Completion First

### Configuración C: RR(3), RR(5), RR(6), RR(20)
- Cola 1: Round Robin con quantum 3
- Cola 2: Round Robin con quantum 5
- Cola 3: Round Robin con quantum 6
- Cola 4: Round Robin con quantum 20

## Archivos de Salida

### Archivo de resultados (_ConfigX.txt)
```
etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT
A;6;0;1;5;8;14;0;14
B;9;0;1;4;14;23;2;23
...
# PROMEDIOS
# Waiting Time (WT): 12.40
# Completion Time (CT): 18.60
# Response Time (RT): 2.80
# Turnaround Time (TAT): 18.60
```

### Archivo de reporte detallado (_ConfigX_detalle.txt)
```
Proceso;Tiempo_Inicio;Tiempo_Fin;Nivel_Cola;Algoritmo
A;0;1;1;RR(1)
B;1;2;1;RR(1)
A;2;5;2;RR(3)
...
```

## Métricas Calculadas

- **WT (Waiting Time)**: Tiempo que el proceso espera en colas
- **CT (Completion Time)**: Tiempo en que el proceso termina
- **RT (Response Time)**: Tiempo desde llegada hasta primera ejecución
- **TAT (Turnaround Time)**: Tiempo total desde llegada hasta finalización

## Ejemplo de Uso

1. Compilar el proyecto:
   ```bash
   make
   ```

2. Ejecutar con archivo de prueba:
   ```bash
   ./mlfq_scheduler
   # Ingrese: Pruebas/mlq001.txt
   ```

3. Seleccionar opción del menú (1-6)

4. Revisar archivos de salida generados

## Características Técnicas

- **Lenguaje**: C++11
- **Paradigma**: Programación Orientada a Objetos
- **Algoritmos soportados**: Round Robin, SJF, STCF
- **Manejo de archivos**: Lectura de entrada y escritura de resultados
- **Validación**: Verificación de formato de entrada y datos

## Archivos de Prueba

Los archivos en la carpeta `Pruebas/` contienen diferentes escenarios:
- `mlq001.txt`: Caso básico
- `mlq002.txt`: Llegadas escalonadas
- `mlq003.txt`: Procesos con diferentes tiempos
- Y más...

## Limpieza

Para limpiar archivos generados:
```bash
make clean        # Elimina todos los archivos generados
make clean-obj    # Elimina solo archivos objeto
```

## Notas Importantes

1. Los procesos se mueven a colas de menor prioridad si no terminan en su quantum (solo en Round Robin)
2. SJF y STCF ejecutan procesos hasta completar
3. El planificador siempre selecciona de la cola de mayor prioridad disponible
4. Los tiempos se calculan automáticamente al finalizar la simulación

## Autor

Implementación del algoritmo MLFQ según especificaciones del curso de Sistemas Operativos.