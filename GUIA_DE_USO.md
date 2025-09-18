# Simulador Multi-Level Feedback Queue (MLFQ) - Guía de Uso

## Descripción General

Esta implementación de un simulador Multi-Level Feedback Queue (MLFQ) está desarrollada en C++ usando programación orientada a objetos. El sistema permite simular diferentes configuraciones de colas con algoritmos de planificación Round Robin (RR), Shortest Job First (SJF) y Shortest Time to Completion First (STCF).

## Estructura del Proyecto

```
- Proceso.h/cpp         : Clase que representa un proceso individual
- Cola.h/cpp           : Clase que representa cada nivel de la MLFQ
- PlanificadorMLFQ.h/cpp : Clase principal que administra todo el sistema MLFQ
- ArchivoUtils.h/cpp   : Utilidades para lectura y escritura de archivos
- main.cpp             : Programa principal con interfaz de usuario
- Makefile             : Script de compilación
- Pruebas/             : Directorio con archivos de prueba
```

## Compilación

### Usando Makefile:
```bash
make windows          # Compila para Windows (.exe)
make                  # Compilación estándar
make clean            # Limpia archivos generados
```

### Compilación manual:
```bash
g++ -std=c++11 -Wall -Wextra -O2 -o mlfq_scheduler.exe main.cpp Proceso.cpp Cola.cpp PlanificadorMLFQ.cpp ArchivoUtils.cpp
```

## Formato de Archivo de Entrada

Los archivos de entrada deben seguir el formato:
```
etiqueta;burstTime;arrivalTime;queueLevel;priority
```

### Ejemplo:
```
# Comentarios inician con #
A;6;0;1;5
B;9;0;1;4
C;10;0;2;3
D;15;0;2;3
E;8;0;3;2
```

Donde:
- **etiqueta**: Identificador del proceso (A, B, C, etc.)
- **burstTime**: Tiempo de ráfaga del proceso
- **arrivalTime**: Tiempo de llegada del proceso
- **queueLevel**: Nivel inicial de cola (1 = mayor prioridad)
- **priority**: Prioridad del proceso

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

## Uso del Programa Principal

### Ejecución:
```bash
./mlfq_scheduler.exe
```

### Menú de opciones:
1. Ejecutar simulación con configuración A
2. Ejecutar simulación con configuración B
3. Ejecutar simulación con configuración C
4. Ejecutar todas las configuraciones
5. Configuración personalizada
6. Salir

### Ejemplo de uso:
1. Ejecutar el programa
2. Ingresar la ruta del archivo (ej: `Pruebas/mlq001.txt`)
3. Seleccionar la opción deseada (1-6)
4. Ver resultados en pantalla y archivos generados

## Formato de Archivo de Salida

### Archivo de resultados (_resultado.txt):
```
# Resultados de simulación MLFQ
# Formato: etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT
etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT
A;6;0;1;5;22;28;0;28
B;9;0;1;4;24;33;1;33
...

# PROMEDIOS
# Waiting Time (WT): 27.40
# Completion Time (CT): 37.00
# Response Time (RT): 4.40
# Turnaround Time (TAT): 37.00
```

### Archivo detallado (_detalle.txt):
```
# Reporte Detallado de Simulación MLFQ
# Historial de Ejecución
Proceso;Tiempo_Inicio;Tiempo_Fin;Nivel_Cola;Algoritmo
A;0;1;1;RR(1)
B;1;2;1;RR(1)
...

# Métricas Promedio
Promedio_WT;27.40
Promedio_CT;37.00
Promedio_RT;4.40
Promedio_TAT;37.00
```

## Métricas Calculadas

- **WT (Waiting Time)**: Tiempo de espera = TAT - BT
- **CT (Completion Time)**: Tiempo de finalización
- **RT (Response Time)**: Tiempo de respuesta = primera ejecución - arrival time
- **TAT (Turnaround Time)**: Tiempo de retorno = CT - AT

## Reglas del Algoritmo MLFQ

1. **Llegada de procesos**: Los procesos llegan según su `arrivalTime` y se insertan en su cola inicial especificada por `queueLevel`.

2. **Selección de procesos**: Se ejecuta siempre el proceso de la cola de mayor prioridad disponible (número menor).

3. **Round Robin**: Si un proceso no termina en su quantum asignado, se mueve a la siguiente cola de menor prioridad.

4. **SJF/STCF**: Los procesos se ejecutan hasta completarse, ordenados por tiempo de ráfaga restante.

5. **Última cola**: Si un proceso llega a la última cola, permanece ahí hasta completarse.

## Ejemplos de Pruebas

### Prueba básica:
```bash
# Crear archivo test.txt:
A;5;0;1;3
B;3;1;1;2
C;8;2;2;1

# Ejecutar:
./mlfq_scheduler.exe
# Ingresar: test.txt
# Seleccionar: 1 (Configuración A)
```

### Prueba con archivo existente:
```bash
./mlfq_scheduler.exe
# Ingresar: Pruebas/mlq001.txt
# Seleccionar: 4 (Todas las configuraciones)
```

## Archivos de Prueba Incluidos

- `mlq001.txt` - `mlq026.txt`: Casos de prueba con diferentes combinaciones de procesos

## Personalización

Para crear configuraciones personalizadas:
1. Seleccionar opción 5 en el menú principal
2. Especificar número de colas (1-4)
3. Para cada cola, seleccionar algoritmo y quantum (si es RR)

## Depuración

Para activar modo debug en el código:
```cpp
PlanificadorMLFQ planificador(true); // Activar debug
```

Esto mostrará información detallada durante la simulación:
- Estado de las colas en cada tiempo
- Movimientos de procesos entre colas
- Decisiones de planificación

## Notas Técnicas

- El sistema maneja automáticamente la memoria de los procesos
- Los archivos soportan comentarios con `#`
- Se validan los formatos de entrada
- Los tiempos negativos o inválidos son rechazados
- La simulación termina cuando todos los procesos han completado

## Solución de Problemas

### Error "No se pudo encontrar el archivo":
- Verificar que la ruta del archivo sea correcta
- Asegurar que el archivo existe y tiene permisos de lectura

### Formato incorrecto:
- Verificar que cada línea tenga exactamente 5 campos separados por `;`
- Asegurar que todos los valores numéricos sean positivos
- Eliminar espacios extra o caracteres especiales

### Compilación fallida:
- Verificar que g++ esté instalado y sea compatible con C++11
- Asegurar que todos los archivos fuente estén presentes
- Revisar permisos de escritura en el directorio