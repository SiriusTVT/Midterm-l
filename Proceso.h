#ifndef PROCESO_H
#define PROCESO_H

#include <string>

/**
 * Clase que representa un proceso en el sistema de planificación MLFQ
 */
class Proceso {
private:
    std::string etiqueta;           // Identificador del proceso (A, B, C, etc.)
    int burstTime;                  // Tiempo de ráfaga original
    int burstTimeRestante;          // Tiempo de ráfaga restante
    int arrivalTime;                // Tiempo de llegada
    int queueLevel;                 // Nivel de cola inicial
    int priority;                   // Prioridad del proceso
    
    // Tiempos calculados durante la simulación
    int waitingTime;                // Tiempo de espera (WT)
    int completionTime;             // Tiempo de finalización (CT)
    int responseTime;               // Tiempo de respuesta (RT)
    int turnaroundTime;             // Tiempo de retorno (TAT)
    
    // Estados del proceso
    bool hasStarted;                // Si el proceso ya ha comenzado su ejecución
    bool isCompleted;               // Si el proceso ha terminado
    int currentQueueLevel;          // Nivel de cola actual (puede cambiar)

public:
    // Constructor
    Proceso(const std::string& etiqueta, int burstTime, int arrivalTime, 
            int queueLevel, int priority);
    
    // Getters
    std::string getEtiqueta() const;
    int getBurstTime() const;
    int getBurstTimeRestante() const;
    int getArrivalTime() const;
    int getQueueLevel() const;
    int getPriority() const;
    int getWaitingTime() const;
    int getCompletionTime() const;
    int getResponseTime() const;
    int getTurnaroundTime() const;
    bool getHasStarted() const;
    bool getIsCompleted() const;
    int getCurrentQueueLevel() const;
    
    // Setters
    void setBurstTimeRestante(int tiempo);
    void setWaitingTime(int tiempo);
    void setCompletionTime(int tiempo);
    void setResponseTime(int tiempo);
    void setTurnaroundTime(int tiempo);
    void setHasStarted(bool started);
    void setIsCompleted(bool completed);
    void setCurrentQueueLevel(int level);
    
    // Métodos de utilidad
    void ejecutar(int tiempoEjecucion);  // Ejecuta el proceso por un tiempo dado
    void calcularTiempos();              // Calcula TAT basado en CT y AT
    void reset();                        // Reinicia el proceso para nueva simulación
    
    // Sobrecarga de operadores para comparación (útil para algoritmos SJF/STCF)
    bool operator<(const Proceso& other) const;  // Para SJF
    bool operator>(const Proceso& other) const;  // Para priority queues
};

#endif // PROCESO_H