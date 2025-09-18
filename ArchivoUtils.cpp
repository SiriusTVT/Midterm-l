#include "ArchivoUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

/**
 * Lee un archivo de entrada y crea los procesos correspondientes
 */
std::vector<Proceso*> ArchivoUtils::leerArchivoProcesos(const std::string& nombreArchivo) {
    std::vector<Proceso*> procesos;
    std::ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return procesos;
    }
    
    std::string linea;
    int numeroLinea = 0;
    
    while (std::getline(archivo, linea)) {
        numeroLinea++;
        
        // Saltar líneas vacías y comentarios
        linea = trim(linea);
        if (linea.empty() || linea[0] == '#') {
            continue;
        }
        
        try {
            // Dividir la línea por punto y coma
            std::vector<std::string> campos = split(linea, ';');
            
            if (campos.size() != 5) {
                std::cerr << "Advertencia: Línea " << numeroLinea 
                          << " tiene formato incorrecto (esperados 5 campos): " << linea << std::endl;
                continue;
            }
            
            // Extraer y limpiar campos
            std::string etiqueta = trim(campos[0]);
            int burstTime = std::stoi(trim(campos[1]));
            int arrivalTime = std::stoi(trim(campos[2]));
            int queueLevel = std::stoi(trim(campos[3]));
            int priority = std::stoi(trim(campos[4]));
            
            // Validar valores
            if (burstTime <= 0 || arrivalTime < 0 || queueLevel <= 0 || priority <= 0) {
                std::cerr << "Advertencia: Línea " << numeroLinea 
                          << " contiene valores inválidos: " << linea << std::endl;
                continue;
            }
            
            // Crear proceso
            Proceso* proceso = new Proceso(etiqueta, burstTime, arrivalTime, queueLevel, priority);
            procesos.push_back(proceso);
            
        } catch (const std::exception& e) {
            std::cerr << "Error procesando línea " << numeroLinea << ": " << e.what() << std::endl;
            continue;
        }
    }
    
    archivo.close();
    
    std::cout << "Archivo leído exitosamente: " << procesos.size() 
              << " procesos cargados desde " << nombreArchivo << std::endl;
    
    return procesos;
}

/**
 * Escribe los resultados de la simulación a un archivo
 */
void ArchivoUtils::escribirResultados(const std::string& nombreArchivo, 
                                    const std::vector<Proceso*>& procesos,
                                    const PlanificadorMLFQ& planificador) {
    std::ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << nombreArchivo << std::endl;
        return;
    }
    
    // Escribir encabezado
    archivo << "# Resultados de simulación MLFQ" << std::endl;
    archivo << "# Formato: etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT" << std::endl;
    archivo << "etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT" << std::endl;
    
    // Escribir datos de cada proceso
    for (const Proceso* proceso : procesos) {
        archivo << proceso->getEtiqueta() << ";"
                << proceso->getBurstTime() << ";"
                << proceso->getArrivalTime() << ";"
                << proceso->getQueueLevel() << ";"
                << proceso->getPriority() << ";"
                << proceso->getWaitingTime() << ";"
                << proceso->getCompletionTime() << ";"
                << proceso->getResponseTime() << ";"
                << proceso->getTurnaroundTime() << std::endl;
    }
    
    // Escribir promedios
    archivo << std::endl << "# PROMEDIOS" << std::endl;
    archivo << std::fixed << std::setprecision(2);
    archivo << "# Waiting Time (WT): " << planificador.getPromedioWT() << std::endl;
    archivo << "# Completion Time (CT): " << planificador.getPromedioCT() << std::endl;
    archivo << "# Response Time (RT): " << planificador.getPromedioRT() << std::endl;
    archivo << "# Turnaround Time (TAT): " << planificador.getPromedioTAT() << std::endl;
    
    archivo.close();
    
    std::cout << "Resultados escritos en: " << nombreArchivo << std::endl;
}

/**
 * Escribe un reporte detallado con historial de ejecución
 */
void ArchivoUtils::escribirReporteDetallado(const std::string& nombreArchivo,
                                          const PlanificadorMLFQ& planificador) {
    std::ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << nombreArchivo << std::endl;
        return;
    }
    
    archivo << "# Reporte Detallado de Simulación MLFQ" << std::endl;
    archivo << "# Historial de Ejecución" << std::endl;
    archivo << "Proceso;Tiempo_Inicio;Tiempo_Fin;Nivel_Cola;Algoritmo" << std::endl;
    
    std::vector<RegistroEjecucion> historial = planificador.getHistorialEjecucion();
    for (const auto& registro : historial) {
        archivo << registro.etiquetaProceso << ";"
                << registro.tiempoInicio << ";"
                << registro.tiempoFin << ";"
                << registro.nivelCola << ";"
                << registro.algoritmo << std::endl;
    }
    
    archivo << std::endl << "# Métricas Promedio" << std::endl;
    archivo << std::fixed << std::setprecision(2);
    archivo << "Promedio_WT;" << planificador.getPromedioWT() << std::endl;
    archivo << "Promedio_CT;" << planificador.getPromedioCT() << std::endl;
    archivo << "Promedio_RT;" << planificador.getPromedioRT() << std::endl;
    archivo << "Promedio_TAT;" << planificador.getPromedioTAT() << std::endl;
    
    archivo.close();
    
    std::cout << "Reporte detallado escrito en: " << nombreArchivo << std::endl;
}

/**
 * Función auxiliar para limpiar espacios en blanco
 */
std::string ArchivoUtils::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

/**
 * Función auxiliar para dividir una cadena por un delimitador
 */
std::vector<std::string> ArchivoUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

/**
 * Valida que un archivo existe y puede ser leído
 */
bool ArchivoUtils::validarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    bool esValido = archivo.good();
    archivo.close();
    return esValido;
}

/**
 * Genera un nombre de archivo de salida basado en el archivo de entrada
 */
std::string ArchivoUtils::generarNombreSalida(const std::string& archivoEntrada, 
                                            const std::string& sufijo) {
    size_t ultimoPunto = archivoEntrada.find_last_of('.');
    
    if (ultimoPunto != std::string::npos) {
        // Tiene extensión
        std::string base = archivoEntrada.substr(0, ultimoPunto);
        std::string extension = archivoEntrada.substr(ultimoPunto);
        return base + sufijo + extension;
    } else {
        // Sin extensión
        return archivoEntrada + sufijo + ".txt";
    }
}