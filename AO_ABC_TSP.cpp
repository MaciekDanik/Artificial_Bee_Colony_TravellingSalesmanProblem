#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include <string>
#include "distanceMatrix.h"
#include "ABC_Alg.h"
#include "FileHandler.h"
#include "NN_alg.h"

using namespace std;

int main()
{
    ABC_Alg alg;
    NN_alg nn_alg;
    //vector<int> cities = { 10, 25, 50 , 100, 250, 500, 1000, 2000};
    vector<int> cities = {200};

    int num_iterations = 1000000;
    int population_size = 10;

    const int alg_iteration = 1;
    int iter = 0;
    vector<SaveFileInput> saveFileInputBee;
    vector<SaveFileInput> saveFileInputNN;
    
    vector<string> bee_fileNames(cities.size(), "");
    for (int i = 0; i<cities.size(); ++i)
    {
        bee_fileNames[i] = "results_bee_" + to_string(cities[i]) + "_cities.txt";
        //cout << bee_fileNames[i] << endl;
    }

    vector<string> nn_fileNames(cities.size(), "");
    for (int i = 0; i < cities.size(); ++i)
    {
        nn_fileNames[i] = "results_nn_" + to_string(cities[i]) + "_cities.txt";
        //cout << bee_fileNames[i] << endl;
    }

    for (int city : cities)
    {
        //Generowanie macierzy odległości
        auto distance_matrix = generate_distance_matrix(city);        
        vector<City> cities_points = distance_matrix.second;

        vector<double> timesBee;
        vector<double> timesNN;
        vector<double> distanceBee;
        vector<double> distanceNN;
        vector<int> best_route_overall_bee;
        vector<int> best_route_overall_nn;
        double best_distance_bee = std::numeric_limits<double>::max();
        double best_distance_nn = std::numeric_limits<double>::max();

        //algorytm pszczeli
        // tu było bee_iteration -1 nie wiem czemu?
        for (int i = 0; i < alg_iteration; i++)
        {
            auto start = chrono::high_resolution_clock::now();

            auto result = alg.abc_algorithm(distance_matrix.first, num_iterations, population_size);

            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            double time_sec = duration.count() / 1'000'000.0;
            //cout << time_sec << "s\n";
            timesBee.push_back(time_sec);
            distanceBee.push_back(result.second);

            if (result.second < best_distance_bee) {
                best_distance_bee = result.second;
                best_route_overall_bee = result.first;
            }
        }

        save_results_bee(cities_points, best_route_overall_bee, best_distance_bee, bee_fileNames[iter]);

        double avgTime_bee = std::accumulate(timesBee.begin(), timesBee.end(), 0.0) / alg_iteration;
        double avgDistance = std::accumulate(distanceBee.begin(), distanceBee.end(), 0.0) / alg_iteration;

        SaveFileInput resultInputBee;
        resultInputBee.cities = cities_points;
        resultInputBee.best_route = best_route_overall_bee;
        resultInputBee.avaregeDitance = avgDistance;
        resultInputBee.avarageTime = avgTime_bee;

        saveFileInputBee.push_back(resultInputBee);

        //Algorytm najbliższych somsiadów
        for (int i = 0; i < alg_iteration; ++i)
        {
            auto start = chrono::high_resolution_clock::now();

            auto result = nn_alg.nn_algorithm(city, distance_matrix.first);

            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            double time_sec = duration.count() / 1'000'000.0;

            timesNN.push_back(time_sec);
            distanceNN.push_back(result.second);

            if (result.second < best_distance_nn) {
                best_distance_nn = result.second;
                best_route_overall_nn = result.first;
            }
        }

        double avgTime_nn = std::accumulate(timesNN.begin(), timesNN.end(), 0.0) / alg_iteration;
        double avgDistance_NN = std::accumulate(distanceNN.begin(), distanceNN.end(), 0.0) / alg_iteration;

        SaveFileInput resultInputNN;
        resultInputNN.cities = cities_points;
        resultInputNN.best_route = best_route_overall_nn;
        resultInputNN.avaregeDitance = avgDistance_NN;
        resultInputNN.avarageTime = avgTime_nn;
        saveFileInputNN.push_back(resultInputNN);

        save_results_nn(cities_points, best_route_overall_nn, best_distance_nn, nn_fileNames[iter]);

        ++iter;

        cout << "Completed for " << city << ". Times: ABC = " << avgTime_bee << "s\tNN = " << avgTime_nn << "s.\n";

    }

    save_multi_results(saveFileInputBee, mulitBeeFileName);
    save_multi_results(saveFileInputNN, mulitNNFileName);


    return 0;
}
