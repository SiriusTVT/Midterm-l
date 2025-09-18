#include "Cola.h"
#include <algorithm>
#include <iostream>

/**
 * Constructor de la clase Cola
 */
Cola::Cola(int nivel, TipoAlgoritmo algoritmo, int quantum) 
    : algoritmo(algoritmo), quantum(quantum), nivel(nivel) {
}

// Getters
TipoAlgoritmo Cola::getAlgoritmo() const {
    return algoritmo;
}

int Cola::getQuantum() const {
    return quantum;
}

int Cola::getNivel() const {
    return nivel;
}

bool Cola::isEmpty() const {
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        return colaProcesos.empty();
    } else {
        return vectorProcesos.empty();
    }
}

size_t Cola::size() const {
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        return colaProcesos.size();
    } else {
        return vectorProcesos.size();
    }
}

/**
 * Agrega un proceso a la cola según el algoritmo
 */
void Cola::agregarProceso(Proceso* proceso) {
    if (proceso == nullptr) return;
    
    // Actualizar el nivel actual del proceso
    proceso->setCurrentQueueLevel(nivel);
    
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        colaProcesos.push(proceso);
    } else {
        vectorProcesos.push_back(proceso);
        // Ordenar inmediatamente para SJF/STCF
        if (algoritmo == TipoAlgoritmo::SJF) {
            ordenarPorSJF();
        } else if (algoritmo == TipoAlgoritmo::STCF) {
            ordenarPorSTCF();
        }
    }
}

/**
 * Obtiene el siguiente proceso a ejecutar según el algoritmo
 */
Proceso* Cola::obtenerSiguienteProceso() {
    if (isEmpty()) {
        return nullptr;
    }
    
    Proceso* proceso = nullptr;
    
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        proceso = colaProcesos.front();
        colaProcesos.pop();
    } else {
        // Para SJF/STCF, tomar el primer elemento (ya ordenado)
        proceso = vectorProcesos.front();
        vectorProcesos.erase(vectorProcesos.begin());
    }
    
    return proceso;
}

/**
 * Devuelve un proceso a la cola (usado en Round Robin cuando no termina en el quantum)
 */
void Cola::devolverProceso(Proceso* proceso) {
    if (proceso == nullptr) return;
    
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        colaProcesos.push(proceso);
    } else {
        // Para SJF/STCF, agregar y reordenar
        vectorProcesos.push_back(proceso);
        if (algoritmo == TipoAlgoritmo::SJF) {
            ordenarPorSJF();
        } else if (algoritmo == TipoAlgoritmo::STCF) {
            ordenarPorSTCF();
        }
    }
}

/**
 * Ordena los procesos por Shortest Job First (burst time restante)
 */
void Cola::ordenarPorSJF() {
    std::sort(vectorProcesos.begin(), vectorProcesos.end(),
              [](const Proceso* a, const Proceso* b) {
                  return a->getBurstTimeRestante() < b->getBurstTimeRestante();
              });
}

/**
 * Ordena los procesos por Shortest Time to Completion First
 * En este contexto, es similar a SJF pero puede cambiar dinámicamente
 */
void Cola::ordenarPorSTCF() {
    std::sort(vectorProcesos.begin(), vectorProcesos.end(),
              [](const Proceso* a, const Proceso* b) {
                  return a->getBurstTimeRestante() < b->getBurstTimeRestante();
              });
}

/**
 * Obtiene todos los procesos en la cola (para debug o migración)
 */
std::vector<Proceso*> Cola::obtenerTodosProcesos() const {
    std::vector<Proceso*> todos;
    
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        std::queue<Proceso*> copia = colaProcesos;
        while (!copia.empty()) {
            todos.push_back(copia.front());
            copia.pop();
        }
    } else {
        todos = vectorProcesos;
    }
    
    return todos;
}

/**
 * Limpia la cola de todos los procesos
 */
void Cola::limpiar() {
    if (algoritmo == TipoAlgoritmo::ROUND_ROBIN) {
        while (!colaProcesos.empty()) {
            colaProcesos.pop();
        }
    } else {
        vectorProcesos.clear();
    }
}

/**
 * Obtiene el nombre del algoritmo como string
 */
std::string Cola::obtenerNombreAlgoritmo() const {
    switch (algoritmo) {
        case TipoAlgoritmo::ROUND_ROBIN:
            return "RR(" + std::to_string(quantum) + ")";
        case TipoAlgoritmo::SJF:
            return "SJF";
        case TipoAlgoritmo::STCF:
            return "STCF";
        default:
            return "UNKNOWN";
    }
}

/**
 * Muestra el estado actual de la cola (para debug)
 */
void Cola::mostrarEstado() const {
    std::cout << "Cola Nivel " << nivel << " (" << obtenerNombreAlgoritmo() << "): ";
    
    if (isEmpty()) {
        std::cout << "VACÍA" << std::endl;
        return;
    }
    
    std::vector<Proceso*> procesos = obtenerTodosProcesos();
    for (const auto& proceso : procesos) {
        std::cout << proceso->getEtiqueta() << "(" << proceso->getBurstTimeRestante() << ") ";
    }
    std::cout << std::endl;
}