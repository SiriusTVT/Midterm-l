#ifndef ARCHIVO_UTILS_H
#define ARCHIVO_UTILS_H

#include <vector>
#include <string>
#include "Proceso.h"
#include "PlanificadorMLFQ.h"

/**
 * Clase utilitaria para manejo de archivos de entrada y salida
 */
class ArchivoUtils {
public:
    /**
     * Lee un archivo de entrada y crea los procesos correspondientes
     * Formato esperado: etiqueta;burstTime;arrivalTime;queueLevel;priority
     * @param nombreArchivo: ruta del archivo a leer
     * @return vector de punteros a procesos creados
     */
    static std::vector<Proceso*> leerArchivoProcesos(const std::string& nombreArchivo);
    
    /**
     * Escribe los resultados de la simulación a un archivo
     * Formato: etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT
     * @param nombreArchivo: ruta del archivo de salida
     * @param procesos: vector de procesos con métricas calculadas
     * @param planificador: referencia al planificador para obtener promedios
     */
    static void escribirResultados(const std::string& nombreArchivo, 
                                 const std::vector<Proceso*>& procesos,
                                 const PlanificadorMLFQ& planificador);
    
    /**
     * Escribe un reporte detallado con historial de ejecución
     * @param nombreArchivo: ruta del archivo de reporte
     * @param planificador: referencia al planificador
     */
    static void escribirReporteDetallado(const std::string& nombreArchivo,
                                       const PlanificadorMLFQ& planificador);
    
    /**
     * Función auxiliar para limpiar espacios en blanco de una cadena
     * @param str: cadena a limpiar
     * @return cadena sin espacios al inicio y final
     */
    static std::string trim(const std::string& str);
    
    /**
     * Función auxiliar para dividir una cadena por un delimitador
     * @param str: cadena a dividir
     * @param delimiter: carácter delimitador
     * @return vector de subcadenas
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);
    
    /**
     * Valida que un archivo existe y puede ser leído
     * @param nombreArchivo: ruta del archivo
     * @return true si el archivo es válido
     */
    static bool validarArchivo(const std::string& nombreArchivo);
    
    /**
     * Genera un nombre de archivo de salida basado en el archivo de entrada
     * @param archivoEntrada: nombre del archivo de entrada
     * @param sufijo: sufijo adicional para el archivo de salida
     * @return nombre del archivo de salida
     */
    static std::string generarNombreSalida(const std::string& archivoEntrada, 
                                         const std::string& sufijo = "_resultado");
};

#endif // ARCHIVO_UTILS_H