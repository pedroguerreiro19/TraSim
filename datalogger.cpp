#include "DataLogger.h"
#include "Graph.h"        // Precisamos da definição de Graph apenas para a forward declaration
#include <iomanip>        // para std::fixed, std::setprecision
#include <stdexcept>      // para std::runtime_error

/**
 * Construtor do DataLogger.
 * Abre o ficheiro CSV, inicializa os acumuladores e escreve o cabeçalho.
 *
 * @param filename    Caminho para o CSV de saída.
 * @param logInterval Intervalo (em segundos de simulação) entre cada gravação.
 * @param graphPtr    Ponteiro para a estrutura principal (Graph) de onde se extraem métricas.
 */
DataLogger::DataLogger(const std::string& filename, double logInterval, Graph* graphPtr)
    : nextLogTime_(logInterval),
    logInterval_(logInterval),
    graph_(graphPtr),
    carsPassedLastInterval_(0),
    sumSpeedLastInterval_(0.0),
    numCarsLastInterval_(0),
    numStoppedLastInterval_(0),
    sumTravelTime_(0.0),
    numFinishedCars_(0)
{
    // Abre o ficheiro CSV para escrita
    outCsv_.open(filename);
    if (!outCsv_.is_open()) {
        throw std::runtime_error("DataLogger: impossível abrir o ficheiro CSV para escrita: " + filename);
    }

    // Escreve o cabeçalho das colunas no CSV
    writeHeader();
}

/**
 * Destrutor do DataLogger.
 * Fecha o ficheiro CSV, caso ainda esteja aberto.
 */
DataLogger::~DataLogger() {
    if (outCsv_.is_open()) {
        outCsv_.close();
    }
}

/**
 * Escreve o cabeçalho (nomes das colunas) no ficheiro CSV.
 * Cada campo corresponde a uma métrica que iremos registar a cada intervalo.
 */
void DataLogger::writeHeader() {
    // Exemplos de colunas:
    // timestamp,location_id,flow_veh_per_hour,avg_speed_m_s,percent_stopped,avg_travel_time_s
    outCsv_ << "timestamp,location_id,flow_veh_per_hour,avg_speed_m_s,percent_stopped,avg_travel_time_s\n";
    // Se quiseres adicionar novas colunas, basta concatenar aqui, separadas por vírgula.
    // Por exemplo: ,density_veh_per_km,queue_length
}

/**
 * update: Deve ser chamado a cada “tick” da simulação.
 * Se currentTime >= nextLogTime_, grava uma linha nova no CSV e reseta os acumuladores.
 *
 * @param currentTime Tempo atual da simulação (em segundos).
 */
void DataLogger::update(double currentTime) {
    // Se já atingimos ou ultrapassamos o próximo momento de gravação…
    if (currentTime >= nextLogTime_) {
        // Escrever as métricas acumuladas até agora no CSV
        writeMetrics(currentTime);

        // Agendar o próximo momento de gravação
        nextLogTime_ += logInterval_;

        // Resetar os acumuladores para o próximo intervalo
        carsPassedLastInterval_ = 0;
        sumSpeedLastInterval_   = 0.0;
        numCarsLastInterval_    = 0;
        numStoppedLastInterval_ = 0;
        sumTravelTime_          = 0.0;
        numFinishedCars_        = 0;
    }
}

/**
 * writeMetrics: Converte os acumuladores em valores formatados e escreve uma linha no CSV.
 * - Flow (veh/h): calcula com base em carsPassedLastInterval_ e logInterval_.
 * - Avg speed (m/s): soma de velocidades / número de carros ativos.
 * - Percent stopped (%): 100 * (nº de carros parados / nº de carros ativos).
 * - Avg travel time (s): soma de tempos de viagem / nº de carros que terminaram.
 *
 * @param currentTime Tempo (segundos) no qual gravamos esta linha.
 */
void DataLogger::writeMetrics(double currentTime) {
    // 1) Timestamp (tempo de simulação) – com 2 casas decimais
    outCsv_ << std::fixed << std::setprecision(2) << currentTime << ",";

    // 2) location_id: se tiveres vários sensores podes modificar aqui.
    // Por agora, deixamos 0 como padrão (por ex., 1 único sensor ou global).
    int locationId = 0;
    outCsv_ << locationId << ",";

    // 3) flow_veh_per_hour:
    // Se passaram N carros em logInterval_ segundos, o fluxo = N / (logInterval_ / 3600.0)
    double flowVehPerHour = 0.0;
    if (logInterval_ > 0.0) {
        flowVehPerHour = static_cast<double>(carsPassedLastInterval_) / (logInterval_ / 3600.0);
    }
    outCsv_ << std::fixed << std::setprecision(2) << flowVehPerHour << ",";

    // 4) avg_speed_m_s: soma das velocidades / número de carros ativos
    double avgSpeed = 0.0;
    if (numCarsLastInterval_ > 0) {
        avgSpeed = sumSpeedLastInterval_ / static_cast<double>(numCarsLastInterval_);
    }
    outCsv_ << std::fixed << std::setprecision(2) << avgSpeed << ",";

    // 5) percent_stopped: 100 * (nº de carros parados / nº de carros ativos)
    double percentStopped = 0.0;
    if (numCarsLastInterval_ > 0) {
        percentStopped = 100.0 * static_cast<double>(numStoppedLastInterval_) / static_cast<double>(numCarsLastInterval_);
    }
    outCsv_ << std::fixed << std::setprecision(2) << percentStopped << ",";

    // 6) avg_travel_time_s: soma dos tempos de viagem / nº de carros que terminaram
    double avgTravelTime = 0.0;
    if (numFinishedCars_ > 0) {
        avgTravelTime = sumTravelTime_ / static_cast<double>(numFinishedCars_);
    }
    outCsv_ << std::fixed << std::setprecision(2) << avgTravelTime;

    // 7) Finalizamos a linha e passamos para a seguinte
    outCsv_ << "\n";
}
