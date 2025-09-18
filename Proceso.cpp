#include "Proceso.h"

/**
 * Constructor de la clase Proceso
 */
Proceso::Proceso(const std::string& etiqueta, int burstTime, int arrivalTime, 
                 int queueLevel, int priority) 
    : etiqueta(etiqueta), burstTime(burstTime), burstTimeRestante(burstTime),
      arrivalTime(arrivalTime), queueLevel(queueLevel), priority(priority),
      waitingTime(0), completionTime(0), responseTime(-1), turnaroundTime(0),
      hasStarted(false), isCompleted(false), currentQueueLevel(queueLevel) {
}

// Getters
std::string Proceso::getEtiqueta() const {
    return etiqueta;
}

int Proceso::getBurstTime() const {
    return burstTime;
}

int Proceso::getBurstTimeRestante() const {
    return burstTimeRestante;
}

int Proceso::getArrivalTime() const {
    return arrivalTime;
}

int Proceso::getQueueLevel() const {
    return queueLevel;
}

int Proceso::getPriority() const {
    return priority;
}

int Proceso::getWaitingTime() const {
    return waitingTime;
}

int Proceso::getCompletionTime() const {
    return completionTime;
}

int Proceso::getResponseTime() const {
    return responseTime;
}

int Proceso::getTurnaroundTime() const {
    return turnaroundTime;
}

bool Proceso::getHasStarted() const {
    return hasStarted;
}

bool Proceso::getIsCompleted() const {
    return isCompleted;
}

int Proceso::getCurrentQueueLevel() const {
    return currentQueueLevel;
}

// Setters
void Proceso::setBurstTimeRestante(int tiempo) {
    burstTimeRestante = tiempo;
}

void Proceso::setWaitingTime(int tiempo) {
    waitingTime = tiempo;
}

void Proceso::setCompletionTime(int tiempo) {
    completionTime = tiempo;
}

void Proceso::setResponseTime(int tiempo) {
    responseTime = tiempo;
}

void Proceso::setTurnaroundTime(int tiempo) {
    turnaroundTime = tiempo;
}

void Proceso::setHasStarted(bool started) {
    hasStarted = started;
}

void Proceso::setIsCompleted(bool completed) {
    isCompleted = completed;
}

void Proceso::setCurrentQueueLevel(int level) {
    currentQueueLevel = level;
}

/**
 * Ejecuta el proceso por un tiempo determinado
 * @param tiempoEjecucion: tiempo que se va a ejecutar el proceso
 */
void Proceso::ejecutar(int tiempoEjecucion) {
    if (!hasStarted) {
        hasStarted = true;
    }
    
    burstTimeRestante -= tiempoEjecucion;
    
    if (burstTimeRestante <= 0) {
        burstTimeRestante = 0;
        isCompleted = true;
    }
}

/**
 * Calcula el tiempo de retorno (TAT) basado en CT y AT
 */
void Proceso::calcularTiempos() {
    if (completionTime > 0) {
        turnaroundTime = completionTime - arrivalTime;
        waitingTime = turnaroundTime - burstTime;
    }
}

/**
 * Reinicia el proceso para una nueva simulación
 */
void Proceso::reset() {
    burstTimeRestante = burstTime;
    waitingTime = 0;
    completionTime = 0;
    responseTime = -1;
    turnaroundTime = 0;
    hasStarted = false;
    isCompleted = false;
    currentQueueLevel = queueLevel;
}

/**
 * Sobrecarga del operador < para comparación en SJF
 * Compara por burst time restante (menor primero)
 */
bool Proceso::operator<(const Proceso& other) const {
    return burstTimeRestante < other.burstTimeRestante;
}

/**
 * Sobrecarga del operador > para priority queues
 * Compara por burst time restante (para min-heap en SJF)
 */
bool Proceso::operator>(const Proceso& other) const {
    return burstTimeRestante > other.burstTimeRestante;
}