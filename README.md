# Multi-Level Feedback Queue (MLFQ) Scheduler

Implementación en C++ de un simulador de planificación Multi-Level Feedback Queue usando programación orientada a objetos.

## Descripción

Este proyecto implementa un algoritmo de planificación MLFQ que permite simular diferentes configuraciones de colas con distintos algoritmos de planificación:

- **Round Robin (RR)** con quantum configurable
- **Shortest Job First (SJF)**
- **Shortest Time to Completion First (STCF)**

## Estructura del Proyecto

```
├── main.cpp                  # Programa principal con interfaz de usuario
├── mlfq_scheduler.exe        # Ejecutable compilado del simulador
├── Proceso.h/.cpp            # Clase Proceso - representa procesos individuales
├── Cola.h/.cpp               # Clase Cola - maneja algoritmos de planificación
├── PlanificadorMLFQ.h/.cpp   # Clase principal del planificador MLFQ
├── ArchivoUtils.h/.cpp       # Utilidades para lectura/escritura de archivos
├── Makefile                  # Script de compilación automatizada
├── README.md                 # Documentación principal del proyecto
├── README_MLFQ.md            # Documentación específica del algoritmo MLFQ
├── GUIA_DE_USO.md            # Guía detallada de uso del simulador
├── SO-P1-2025-II.pdf         # Documento original del proyecto
└── Pruebas/                  # Directorio con archivos de casos de prueba
    ├── mlq001.txt            # Caso de prueba 1
    ├── mlq002.txt            # Caso de prueba 2
    ├── mlq003.txt            # Caso de prueba 3
    ├── mlq004.txt            # Caso de prueba 4
    ├── mlq005.txt            # Caso de prueba 5
    ├── mlq006.txt            # Caso de prueba 6
    ├── mlq007.txt            # Caso de prueba 7
    ├── mlq010.txt            # Caso de prueba 10
    ├── mlq014.txt            # Caso de prueba 14
    ├── mlq019.txt            # Caso de prueba 19
    ├── mlq021.txt            # Caso de prueba 21
    ├── mlq025.txt            # Caso de prueba 25
    └── mlq026.txt            # Caso de prueba 26
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
# En Windows (ejecutable ya compilado):
./mlfq_scheduler.exe

# En Linux/Mac:
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

1. **Opción 1 - Usar ejecutable ya compilado**:
   ```bash
   # En Windows:
   ./mlfq_scheduler.exe
   ```

2. **Opción 2 - Recompilar desde código fuente**:
   ```bash
   make clean    # Limpiar archivos previos
   make windows  # Compilar para Windows
   ```

3. **Ejecutar simulación**:
   ```bash
   # Ingrese ruta del archivo cuando se solicite:
   Pruebas/mlq001.txt
   
   # Seleccione opción del menú (1-6):
   1  # Para Configuración A
   ```

4. **Revisar resultados**:
   - Archivo de resultados: `Pruebas/mlq001_ConfigA.txt`
   - Reporte detallado: `Pruebas/mlq001_ConfigA_detalle.txt`

## Características Técnicas

- **Lenguaje**: C++
- **Paradigma**: Programación Orientada a Objetos
- **Algoritmos soportados**: Round Robin, SJF, STCF
- **Manejo de archivos**: Lectura de entrada y escritura de resultados
- **Validación**: Verificación de formato de entrada y datos

## Archivos de Prueba

Los archivos en la carpeta `Pruebas/` contienen diferentes escenarios:
- `mlq001.txt`: Caso básico con 5 procesos
- `mlq002.txt`: Llegadas escalonadas
- `mlq003.txt`: Procesos con diferentes tiempos de ráfaga
- `mlq004.txt` - `mlq026.txt`: Casos adicionales con variaciones

## Documentación Adicional

- **`GUIA_DE_USO.md`**: Guía completa de instalación, compilación y uso
- **`README_MLFQ.md`**: Documentación técnica específica del algoritmo MLFQ
- **`SO-P1-2025-II.pdf`**: Documento original del proyecto

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