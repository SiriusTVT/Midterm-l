#ifndef PLANIFICADOR_MLFQ_H
#define PLANIFICADOR_MLFQ_H

#include <vector>
#include <string>
#include <queue>
#include "Proceso.h"
#include "Cola.h"

/**
 * Estructura para registrar la ejecución de procesos
 */
struct RegistroEjecucion {
    std::string etiquetaProceso;
    int tiempoInicio;
    int tiempoFin;
    int nivelCola;
    std::string algoritmo;
};

/**
 * Estructura para configurar cada cola del MLFQ
 */
struct ConfiguracionCola {
    int nivel;
    TipoAlgoritmo algoritmo;
    int quantum;  // Solo usado para Round Robin
    
    ConfiguracionCola(int n, TipoAlgoritmo alg, int q = 1) 
        : nivel(n), algoritmo(alg), quantum(q) {}
};

/**
 * Clase principal que implementa el Multi-Level Feedback Queue
 */
class PlanificadorMLFQ {
private:
    std::vector<Cola*> colas;                           // Colas del MLFQ ordenadas por prioridad
    std::vector<Proceso*> procesos;                     // Lista de todos los procesos
    std::vector<RegistroEjecucion> historialEjecucion; // Registro de ejecución
    std::queue<Proceso*> procesosLlegada;               // Cola de procesos por llegar
    
    int tiempoActual;                                   // Tiempo actual de la simulación
    bool modoDebug;                                     // Para mostrar información detallada
    
    // Métricas calculadas
    double promedioWT;
    double promedioCT;
    double promedioRT;
    double promedioTAT;
    
public:
    // Constructor y destructor
    PlanificadorMLFQ(bool debug = false);
    ~PlanificadorMLFQ();
    
    // Configuración del planificador
    void configurarColas(const std::vector<ConfiguracionCola>& configuraciones);
    void agregarProceso(Proceso* proceso);
    void limpiarProcesos();
    
    // Métodos principales de simulación
    void ejecutarSimulacion();
    void procesarLlegadas();                            // Procesa procesos que llegan en tiempo actual
    Proceso* seleccionarProcesoParaEjecutar();         // Selecciona proceso de mayor prioridad
    void ejecutarProceso(Proceso* proceso);            // Ejecuta un proceso según su algoritmo
    void moverProcesoASiguienteCola(Proceso* proceso); // Mueve proceso a cola de menor prioridad
    
    // Cálculo de métricas
    void calcularMetricas();
    void mostrarResultados() const;
    void mostrarEstadoColas() const;
    
    // Getters para métricas
    double getPromedioWT() const;
    double getPromedioCT() const;
    double getPromedioRT() const;
    double getPromedioTAT() const;
    
    // Métodos de utilidad
    void reiniciarSimulacion();
    std::vector<RegistroEjecucion> getHistorialEjecucion() const;
    void setModoDebug(bool debug);
    bool todasColasVacias() const;
    
    // Métodos para diferentes configuraciones predefinidas
    void configuracionA();  // RR(1), RR(3), RR(4), SJF
    void configuracionB();  // RR(2), RR(3), RR(4), STCF
    void configuracionC();  // RR(3), RR(5), RR(6), RR(20)
};

#endif // PLANIFICADOR_MLFQ_H