#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Proceso.h"
#include "Cola.h"
#include "PlanificadorMLFQ.h"
#include "ArchivoUtils.h"

/**
 * Función para mostrar el menú de opciones
 */
void mostrarMenu() {
    std::cout << "\n=== SIMULADOR MULTI-LEVEL FEEDBACK QUEUE (MLFQ) ===" << std::endl;
    std::cout << "1. Ejecutar simulación con configuración A: RR(1), RR(3), RR(4), SJF" << std::endl;
    std::cout << "2. Ejecutar simulación con configuración B: RR(2), RR(3), RR(4), STCF" << std::endl;
    std::cout << "3. Ejecutar simulación con configuración C: RR(3), RR(5), RR(6), RR(20)" << std::endl;
    std::cout << "4. Ejecutar todas las configuraciones" << std::endl;
    std::cout << "5. Configuración personalizada" << std::endl;
    std::cout << "6. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

/**
 * Función para ejecutar una simulación con una configuración específica
 */
void ejecutarSimulacion(const std::string& archivoEntrada, const std::string& nombreConfig, 
                       int tipoConfig, bool modoDebug = false) {
    
    std::cout << "\n--- Ejecutando " << nombreConfig << " ---" << std::endl;
    
    // Leer procesos del archivo
    std::vector<Proceso*> procesos = ArchivoUtils::leerArchivoProcesos(archivoEntrada);
    
    if (procesos.empty()) {
        std::cout << "No se pudieron cargar procesos del archivo." << std::endl;
        return;
    }
    
    // Crear planificador
    PlanificadorMLFQ planificador(modoDebug);
    
    // Configurar según el tipo
    switch (tipoConfig) {
        case 1:
            planificador.configuracionA();
            break;
        case 2:
            planificador.configuracionB();
            break;
        case 3:
            planificador.configuracionC();
            break;
        default:
            std::cout << "Configuración no válida." << std::endl;
            return;
    }
    
    // Agregar procesos al planificador
    for (Proceso* proceso : procesos) {
        planificador.agregarProceso(proceso);
    }
    
    // Ejecutar simulación
    planificador.ejecutarSimulacion();
    
    // Generar archivos de salida
    std::string archivoSalida = ArchivoUtils::generarNombreSalida(archivoEntrada, "_" + nombreConfig);
    std::string archivoReporte = ArchivoUtils::generarNombreSalida(archivoEntrada, "_" + nombreConfig + "_detalle");
    
    ArchivoUtils::escribirResultados(archivoSalida, procesos, planificador);
    ArchivoUtils::escribirReporteDetallado(archivoReporte, planificador);
    
    // Mostrar resultados en consola
    planificador.mostrarResultados();
    
    // Limpiar memoria
    planificador.limpiarProcesos();
}

/**
 * Función para configuración personalizada
 */
void configuracionPersonalizada(const std::string& archivoEntrada) {
    std::cout << "\n=== CONFIGURACIÓN PERSONALIZADA ===" << std::endl;
    
    int numColas;
    std::cout << "Ingrese el número de colas (1-4): ";
    std::cin >> numColas;
    
    if (numColas < 1 || numColas > 4) {
        std::cout << "Número de colas inválido." << std::endl;
        return;
    }
    
    std::vector<ConfiguracionCola> configuraciones;
    
    for (int i = 1; i <= numColas; i++) {
        std::cout << "\nCola " << i << ":" << std::endl;
        std::cout << "1. Round Robin" << std::endl;
        std::cout << "2. SJF" << std::endl;
        std::cout << "3. STCF" << std::endl;
        
        int tipoAlgoritmo;
        std::cout << "Seleccione algoritmo: ";
        std::cin >> tipoAlgoritmo;
        
        TipoAlgoritmo algoritmo;
        int quantum = 1;
        
        switch (tipoAlgoritmo) {
            case 1:
                algoritmo = TipoAlgoritmo::ROUND_ROBIN;
                std::cout << "Ingrese quantum para Round Robin: ";
                std::cin >> quantum;
                break;
            case 2:
                algoritmo = TipoAlgoritmo::SJF;
                break;
            case 3:
                algoritmo = TipoAlgoritmo::STCF;
                break;
            default:
                std::cout << "Algoritmo inválido, usando Round Robin con quantum 1." << std::endl;
                algoritmo = TipoAlgoritmo::ROUND_ROBIN;
                quantum = 1;
        }
        
        configuraciones.push_back(ConfiguracionCola(i, algoritmo, quantum));
    }
    
    // Ejecutar simulación personalizada
    std::vector<Proceso*> procesos = ArchivoUtils::leerArchivoProcesos(archivoEntrada);
    
    if (procesos.empty()) {
        std::cout << "No se pudieron cargar procesos del archivo." << std::endl;
        return;
    }
    
    PlanificadorMLFQ planificador(true); // Debug activado para configuración personalizada
    planificador.configurarColas(configuraciones);
    
    for (Proceso* proceso : procesos) {
        planificador.agregarProceso(proceso);
    }
    
    planificador.ejecutarSimulacion();
    
    // Generar archivo de salida
    std::string archivoSalida = ArchivoUtils::generarNombreSalida(archivoEntrada, "_personalizado");
    ArchivoUtils::escribirResultados(archivoSalida, procesos, planificador);
    
    planificador.limpiarProcesos();
}

/**
 * Función principal
 */
int main() {
    std::string archivoEntrada;
    
    std::cout << "=== SIMULADOR MLFQ ===" << std::endl;
    std::cout << "Ingrese la ruta del archivo de entrada: ";
    std::getline(std::cin, archivoEntrada);
    
    // Validar archivo
    if (!ArchivoUtils::validarArchivo(archivoEntrada)) {
        std::cout << "Error: No se pudo encontrar o leer el archivo: " << archivoEntrada << std::endl;
        return 1;
    }
    
    int opcion;
    bool continuar = true;
    
    while (continuar) {
        mostrarMenu();
        std::cin >> opcion;
        
        switch (opcion) {
            case 1:
                ejecutarSimulacion(archivoEntrada, "ConfigA", 1);
                break;
            
            case 2:
                ejecutarSimulacion(archivoEntrada, "ConfigB", 2);
                break;
            
            case 3:
                ejecutarSimulacion(archivoEntrada, "ConfigC", 3);
                break;
            
            case 4:
                std::cout << "\n=== EJECUTANDO TODAS LAS CONFIGURACIONES ===" << std::endl;
                ejecutarSimulacion(archivoEntrada, "ConfigA", 1);
                ejecutarSimulacion(archivoEntrada, "ConfigB", 2);
                ejecutarSimulacion(archivoEntrada, "ConfigC", 3);
                
                // Mostrar comparación
                std::cout << "\n=== COMPARACIÓN DE CONFIGURACIONES ===" << std::endl;
                std::cout << "Ver archivos de salida generados para comparar métricas." << std::endl;
                break;
            
            case 5:
                configuracionPersonalizada(archivoEntrada);
                break;
            
            case 6:
                continuar = false;
                std::cout << "¡Gracias por usar el simulador MLFQ!" << std::endl;
                break;
            
            default:
                std::cout << "Opción no válida. Intente nuevamente." << std::endl;
        }
        
        if (continuar && opcion >= 1 && opcion <= 5) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
    }
    
    return 0;
}