#ifndef COLA_H
#define COLA_H

#include <queue>
#include <vector>
#include <string>
#include "Proceso.h"

/**
 * Enumeración para los tipos de algoritmos de planificación
 */
enum class TipoAlgoritmo {
    ROUND_ROBIN,    // Round Robin con quantum
    SJF,           // Shortest Job First
    STCF           // Shortest Time to Completion First
};

/**
 * Clase que representa una cola de procesos con un algoritmo específico
 */
class Cola {
private:
    TipoAlgoritmo algoritmo;        // Tipo de algoritmo de esta cola
    int quantum;                    // Quantum para Round Robin (no usado en SJF/STCF)
    int nivel;                      // Nivel de la cola (1 = mayor prioridad)
    std::queue<Proceso*> colaProcesos;      // Cola FIFO para Round Robin
    std::vector<Proceso*> vectorProcesos;   // Vector para SJF/STCF (necesita ordenamiento)
    
public:
    // Constructores
    Cola(int nivel, TipoAlgoritmo algoritmo, int quantum = 1);
    
    // Getters
    TipoAlgoritmo getAlgoritmo() const;
    int getQuantum() const;
    int getNivel() const;
    bool isEmpty() const;
    size_t size() const;
    
    // Métodos principales
    void agregarProceso(Proceso* proceso);
    Proceso* obtenerSiguienteProceso();
    void devolverProceso(Proceso* proceso);  // Para Round Robin cuando no termina en quantum
    void ordenarPorSJF();                    // Ordena por burst time restante
    void ordenarPorSTCF();                   // Ordena por tiempo restante
    
    // Métodos de utilidad
    std::vector<Proceso*> obtenerTodosProcesos() const;
    void limpiar();
    std::string obtenerNombreAlgoritmo() const;
    
    // Método para mostrar estado de la cola (debug)
    void mostrarEstado() const;
};

#endif // COLA_H