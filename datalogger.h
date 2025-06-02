#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <fstream>
#include <string>

// Declarações adiantadas (forward declarations) das classes que o DataLogger irá usar
class Graph;

/**
 * @brief DataLogger abre um ficheiro CSV e, a cada intervalo de simulação,
 * recolhe estatísticas do grafo e escreve uma linha no CSV.
 */
class DataLogger {
public:
    /**
     * @param filename: caminho para o CSV de saída
     * @param logInterval: intervalo (em segundos de simulação) entre cada gravação
     * @param graphPtr: ponteiro para a estrutura principal (Graph) de onde se extraem métricas
     */
    DataLogger(const std::string& filename, double logInterval, Graph* graphPtr);

    ~DataLogger();

    /**
     * Deve ser chamado a cada “frame” ou sempre que o tempo simulado ultrapasse
     * (n * logInterval). Ex.: se logInterval = 60s, chamá-lo quando currentTime >= 60, 120, 180…
     */
    void update(double currentTime);

private:
    void writeHeader();
    void writeMetrics(double currentTime);

    std::ofstream outCsv_;          // stream para escrita no ficheiro CSV
    double nextLogTime_;            // próximo instante (simulado) em que gravamos
    double logInterval_;            // intervalo (em segundos) entre gravações
    Graph* graph_;                  // ponteiro para o grafo principal

    // Variáveis para acumular métricas entre intervalos de gravação:
    int carsPassedLastInterval_;    // nº de carros que passaram num sensor ou terminaram rota
    double sumSpeedLastInterval_;   // soma das velocidades de todos os carros ativos
    int numCarsLastInterval_;       // nº de carros ativos considerados (para avg speed)
    int numStoppedLastInterval_;    // nº de carros parados no momento da gravação
    double sumTravelTime_;          // soma dos tempos de viagem dos carros que terminaram
    int numFinishedCars_;           // nº de carros que completaram percurso no intervalo
};

#endif // DATALOGGER_H
