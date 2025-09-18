# Makefile para el proyecto MLFQ Scheduler
# Compilador y banderas
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Archivos fuente y objeto
SOURCES = main.cpp Proceso.cpp Cola.cpp PlanificadorMLFQ.cpp ArchivoUtils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = mlfq_scheduler

# Regla principal
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Reglas para archivos objeto
main.o: main.cpp Proceso.h Cola.h PlanificadorMLFQ.h ArchivoUtils.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Proceso.o: Proceso.cpp Proceso.h
	$(CXX) $(CXXFLAGS) -c Proceso.cpp

Cola.o: Cola.cpp Cola.h Proceso.h
	$(CXX) $(CXXFLAGS) -c Cola.cpp

PlanificadorMLFQ.o: PlanificadorMLFQ.cpp PlanificadorMLFQ.h Proceso.h Cola.h
	$(CXX) $(CXXFLAGS) -c PlanificadorMLFQ.cpp

ArchivoUtils.o: ArchivoUtils.cpp ArchivoUtils.h Proceso.h PlanificadorMLFQ.h
	$(CXX) $(CXXFLAGS) -c ArchivoUtils.cpp

# Reglas para limpiar archivos generados
clean:
	del *.o $(TARGET).exe 2>nul || echo "Limpieza completada"

# Regla para limpiar solo archivos objeto
clean-obj:
	del *.o 2>nul || echo "Archivos objeto eliminados"

# Reglas para compilación rápida en diferentes sistemas
windows: $(TARGET).exe

$(TARGET).exe: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET).exe $(OBJECTS)

# Regla para ejecutar con un archivo de prueba
test: $(TARGET)
	./$(TARGET) Pruebas/mlq001.txt

# Regla para mostrar ayuda
help:
	@echo "Makefile para MLFQ Scheduler"
	@echo "Uso:"
	@echo "  make          - Compila el proyecto"
	@echo "  make windows  - Compila para Windows (.exe)"
	@echo "  make clean    - Elimina archivos generados"
	@echo "  make clean-obj- Elimina solo archivos objeto"
	@echo "  make test     - Compila y ejecuta con archivo de prueba"
	@echo "  make help     - Muestra esta ayuda"

.PHONY: all clean clean-obj test help windows