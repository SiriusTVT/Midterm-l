#include "PlanificadorMLFQ.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

/**
 * Constructor del PlanificadorMLFQ
 */
PlanificadorMLFQ::PlanificadorMLFQ(bool debug) 
    : tiempoActual(0), modoDebug(debug), promedioWT(0), promedioCT(0), 
      promedioRT(0), promedioTAT(0) {
}

/**
 * Destructor - libera memoria de las colas
 */
PlanificadorMLFQ::~PlanificadorMLFQ() {
    for (Cola* cola : colas) {
        delete cola;
    }
    colas.clear();
}

/**
 * Configura las colas del MLFQ según las especificaciones
 */
void PlanificadorMLFQ::configurarColas(const std::vector<ConfiguracionCola>& configuraciones) {
    // Limpiar colas existentes
    for (Cola* cola : colas) {
        delete cola;
    }
    colas.clear();
    
    // Crear nuevas colas
    for (const auto& config : configuraciones) {
        Cola* nuevaCola = new Cola(config.nivel, config.algoritmo, config.quantum);
        colas.push_back(nuevaCola);
    }
    
    if (modoDebug) {
        std::cout << "Configuración de colas:" << std::endl;
        for (const auto& cola : colas) {
            std::cout << "  Cola " << cola->getNivel() << ": " 
                      << cola->obtenerNombreAlgoritmo() << std::endl;
        }
    }
}

/**
 * Agrega un proceso al planificador
 */
void PlanificadorMLFQ::agregarProceso(Proceso* proceso) {
    if (proceso != nullptr) {
        procesos.push_back(proceso);
    }
}

/**
 * Limpia todos los procesos del planificador
 */
void PlanificadorMLFQ::limpiarProcesos() {
    for (Proceso* proceso : procesos) {
        delete proceso;
    }
    procesos.clear();
    
    // Limpiar también las colas
    for (Cola* cola : colas) {
        cola->limpiar();
    }
    
    // Limpiar otros contenedores
    while (!procesosLlegada.empty()) {
        procesosLlegada.pop();
    }
    historialEjecucion.clear();
}

/**
 * Ejecuta la simulación completa del MLFQ
 */
void PlanificadorMLFQ::ejecutarSimulacion() {
    if (procesos.empty() || colas.empty()) {
        std::cout << "Error: No hay procesos o colas configuradas." << std::endl;
        return;
    }
    
    // Reiniciar estado
    reiniciarSimulacion();
    
    // Ordenar procesos por tiempo de llegada
    std::sort(procesos.begin(), procesos.end(),
              [](const Proceso* a, const Proceso* b) {
                  return a->getArrivalTime() < b->getArrivalTime();
              });
    
    // Colocar procesos en cola de llegada
    for (Proceso* proceso : procesos) {
        procesosLlegada.push(proceso);
    }
    
    if (modoDebug) {
        std::cout << "\n=== INICIANDO SIMULACIÓN MLFQ ===" << std::endl;
    }
    
    // Bucle principal de simulación
    while (!procesosLlegada.empty() || !todasColasVacias()) {
        if (modoDebug) {
            std::cout << "\n--- Tiempo " << tiempoActual << " ---" << std::endl;
        }
        
        // Procesar llegadas de procesos
        procesarLlegadas();
        
        // Seleccionar proceso para ejecutar
        Proceso* procesoActual = seleccionarProcesoParaEjecutar();
        
        if (procesoActual != nullptr) {
            // Ejecutar proceso
            ejecutarProceso(procesoActual);
        } else {
            // No hay procesos listos, avanzar tiempo
            tiempoActual++;
        }
        
        if (modoDebug) {
            mostrarEstadoColas();
        }
    }
    
    // Calcular métricas finales
    calcularMetricas();
    
    if (modoDebug) {
        std::cout << "\n=== SIMULACIÓN COMPLETADA ===" << std::endl;
        mostrarResultados();
    }
}

/**
 * Procesa los procesos que llegan en el tiempo actual
 */
void PlanificadorMLFQ::procesarLlegadas() {
    while (!procesosLlegada.empty() && 
           procesosLlegada.front()->getArrivalTime() <= tiempoActual) {
        
        Proceso* proceso = procesosLlegada.front();
        procesosLlegada.pop();
        
        // Agregar proceso a su cola inicial
        int nivelInicial = proceso->getQueueLevel() - 1; // Convertir a índice (0-based)
        if (nivelInicial >= 0 && nivelInicial < static_cast<int>(colas.size())) {
            colas[nivelInicial]->agregarProceso(proceso);
            
            if (modoDebug) {
                std::cout << "Proceso " << proceso->getEtiqueta() 
                          << " llega y se agrega a cola " << (nivelInicial + 1) << std::endl;
            }
        }
    }
}

/**
 * Selecciona el próximo proceso a ejecutar (cola de mayor prioridad)
 */
Proceso* PlanificadorMLFQ::seleccionarProcesoParaEjecutar() {
    for (Cola* cola : colas) {
        if (!cola->isEmpty()) {
            return cola->obtenerSiguienteProceso();
        }
    }
    return nullptr;
}

/**
 * Ejecuta un proceso según el algoritmo de su cola
 */
void PlanificadorMLFQ::ejecutarProceso(Proceso* proceso) {
    if (proceso == nullptr) return;
    
    // Establecer tiempo de respuesta si es la primera vez que se ejecuta
    if (!proceso->getHasStarted()) {
        proceso->setResponseTime(tiempoActual - proceso->getArrivalTime());
    }
    
    Cola* colaActual = colas[proceso->getCurrentQueueLevel() - 1];
    int tiempoEjecucion = 0;
    int tiempoInicio = tiempoActual;
    
    if (colaActual->getAlgoritmo() == TipoAlgoritmo::ROUND_ROBIN) {
        // Round Robin: ejecutar por quantum o hasta completar
        tiempoEjecucion = std::min(colaActual->getQuantum(), proceso->getBurstTimeRestante());
        proceso->ejecutar(tiempoEjecucion);
        tiempoActual += tiempoEjecucion;
        
        if (proceso->getIsCompleted()) {
            // Proceso completado
            proceso->setCompletionTime(tiempoActual);
            proceso->calcularTiempos();
            
            if (modoDebug) {
                std::cout << "Proceso " << proceso->getEtiqueta() 
                          << " COMPLETADO en tiempo " << tiempoActual << std::endl;
            }
        } else {
            // Proceso no completado, mover a siguiente cola
            moverProcesoASiguienteCola(proceso);
            
            if (modoDebug) {
                std::cout << "Proceso " << proceso->getEtiqueta() 
                          << " no completó en quantum, movido a cola " 
                          << proceso->getCurrentQueueLevel() << std::endl;
            }
        }
        
    } else {
        // SJF/STCF: ejecutar hasta completar
        tiempoEjecucion = proceso->getBurstTimeRestante();
        proceso->ejecutar(tiempoEjecucion);
        tiempoActual += tiempoEjecucion;
        
        proceso->setCompletionTime(tiempoActual);
        proceso->calcularTiempos();
        
        if (modoDebug) {
            std::cout << "Proceso " << proceso->getEtiqueta() 
                      << " ejecutado completamente (" << colaActual->obtenerNombreAlgoritmo() 
                      << ") en tiempo " << tiempoActual << std::endl;
        }
    }
    
    // Registrar ejecución
    RegistroEjecucion registro;
    registro.etiquetaProceso = proceso->getEtiqueta();
    registro.tiempoInicio = tiempoInicio;
    registro.tiempoFin = tiempoActual;
    registro.nivelCola = proceso->getCurrentQueueLevel();
    registro.algoritmo = colaActual->obtenerNombreAlgoritmo();
    historialEjecucion.push_back(registro);
}

/**
 * Mueve un proceso a la siguiente cola de menor prioridad
 */
void PlanificadorMLFQ::moverProcesoASiguienteCola(Proceso* proceso) {
    int nivelActual = proceso->getCurrentQueueLevel() - 1; // Convertir a índice
    int siguienteNivel = nivelActual + 1;
    
    if (siguienteNivel < static_cast<int>(colas.size())) {
        // Mover a siguiente cola
        proceso->setCurrentQueueLevel(siguienteNivel + 1);
        colas[siguienteNivel]->agregarProceso(proceso);
    } else {
        // Ya está en la última cola, regresar a la misma cola
        colas[nivelActual]->devolverProceso(proceso);
    }
}

/**
 * Verifica si todas las colas están vacías
 */
bool PlanificadorMLFQ::todasColasVacias() const {
    for (const Cola* cola : colas) {
        if (!cola->isEmpty()) {
            return false;
        }
    }
    return true;
}

/**
 * Calcula las métricas promedio de la simulación
 */
void PlanificadorMLFQ::calcularMetricas() {
    if (procesos.empty()) return;
    
    double sumaWT = 0, sumaCT = 0, sumaRT = 0, sumaTAT = 0;
    
    for (const Proceso* proceso : procesos) {
        sumaWT += proceso->getWaitingTime();
        sumaCT += proceso->getCompletionTime();
        sumaRT += proceso->getResponseTime();
        sumaTAT += proceso->getTurnaroundTime();
    }
    
    int numProcesos = static_cast<int>(procesos.size());
    promedioWT = sumaWT / numProcesos;
    promedioCT = sumaCT / numProcesos;
    promedioRT = sumaRT / numProcesos;
    promedioTAT = sumaTAT / numProcesos;
}

/**
 * Muestra los resultados de la simulación
 */
void PlanificadorMLFQ::mostrarResultados() const {
    std::cout << "\n=== RESULTADOS DE LA SIMULACIÓN ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    // Tabla de procesos
    std::cout << "\nTabla de Procesos:" << std::endl;
    std::cout << "Etiqueta\tBT\tAT\tQ\tPr\tWT\tCT\tRT\tTAT" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    
    for (const Proceso* proceso : procesos) {
        std::cout << proceso->getEtiqueta() << "\t\t"
                  << proceso->getBurstTime() << "\t"
                  << proceso->getArrivalTime() << "\t"
                  << proceso->getQueueLevel() << "\t"
                  << proceso->getPriority() << "\t"
                  << proceso->getWaitingTime() << "\t"
                  << proceso->getCompletionTime() << "\t"
                  << proceso->getResponseTime() << "\t"
                  << proceso->getTurnaroundTime() << std::endl;
    }
    
    // Promedios
    std::cout << "\n=== PROMEDIOS ===" << std::endl;
    std::cout << "Waiting Time (WT): " << promedioWT << std::endl;
    std::cout << "Completion Time (CT): " << promedioCT << std::endl;
    std::cout << "Response Time (RT): " << promedioRT << std::endl;
    std::cout << "Turnaround Time (TAT): " << promedioTAT << std::endl;
}

/**
 * Muestra el estado actual de todas las colas
 */
void PlanificadorMLFQ::mostrarEstadoColas() const {
    std::cout << "Estado de colas:" << std::endl;
    for (const Cola* cola : colas) {
        cola->mostrarEstado();
    }
}

// Getters para métricas
double PlanificadorMLFQ::getPromedioWT() const { return promedioWT; }
double PlanificadorMLFQ::getPromedioCT() const { return promedioCT; }
double PlanificadorMLFQ::getPromedioRT() const { return promedioRT; }
double PlanificadorMLFQ::getPromedioTAT() const { return promedioTAT; }

/**
 * Reinicia la simulación para ejecutar nuevamente
 */
void PlanificadorMLFQ::reiniciarSimulacion() {
    tiempoActual = 0;
    historialEjecucion.clear();
    
    // Reiniciar todos los procesos
    for (Proceso* proceso : procesos) {
        proceso->reset();
    }
    
    // Limpiar todas las colas
    for (Cola* cola : colas) {
        cola->limpiar();
    }
    
    // Limpiar cola de llegadas
    while (!procesosLlegada.empty()) {
        procesosLlegada.pop();
    }
}

std::vector<RegistroEjecucion> PlanificadorMLFQ::getHistorialEjecucion() const {
    return historialEjecucion;
}

void PlanificadorMLFQ::setModoDebug(bool debug) {
    modoDebug = debug;
}

// Configuraciones predefinidas
void PlanificadorMLFQ::configuracionA() {
    std::vector<ConfiguracionCola> config = {
        ConfiguracionCola(1, TipoAlgoritmo::ROUND_ROBIN, 1),
        ConfiguracionCola(2, TipoAlgoritmo::ROUND_ROBIN, 3),
        ConfiguracionCola(3, TipoAlgoritmo::ROUND_ROBIN, 4),
        ConfiguracionCola(4, TipoAlgoritmo::SJF)
    };
    configurarColas(config);
}

void PlanificadorMLFQ::configuracionB() {
    std::vector<ConfiguracionCola> config = {
        ConfiguracionCola(1, TipoAlgoritmo::ROUND_ROBIN, 2),
        ConfiguracionCola(2, TipoAlgoritmo::ROUND_ROBIN, 3),
        ConfiguracionCola(3, TipoAlgoritmo::ROUND_ROBIN, 4),
        ConfiguracionCola(4, TipoAlgoritmo::STCF)
    };
    configurarColas(config);
}

void PlanificadorMLFQ::configuracionC() {
    std::vector<ConfiguracionCola> config = {
        ConfiguracionCola(1, TipoAlgoritmo::ROUND_ROBIN, 3),
        ConfiguracionCola(2, TipoAlgoritmo::ROUND_ROBIN, 5),
        ConfiguracionCola(3, TipoAlgoritmo::ROUND_ROBIN, 6),
        ConfiguracionCola(4, TipoAlgoritmo::ROUND_ROBIN, 20)
    };
    configurarColas(config);
}