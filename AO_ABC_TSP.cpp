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
    vector<int> cities = { 10, 25, 50 };

    int num_iterations = 2000;
    int population_size = 50;

    const int bee_iteration = 5;
    int iter = 0;
    vector<SaveFileInput> saveFileInputBee;
    vector<SaveFileInput> saveFileInputNN;
    
    vector<string> bee_fileNames(cities.size(), "");
    for (int i = 0; i<cities.size(); ++i)
    {
        bee_fileNames[i] = "results_bee_" + to_string(i) + ".txt";
        //cout << bee_fileNames[i] << endl;
    }

    vector<string> nn_fileNames(cities.size(), "");
    for (int i = 0; i < cities.size(); ++i)
    {
        nn_fileNames[i] = "results_nn_" + to_string(i) + ".txt";
        //cout << bee_fileNames[i] << endl;
    }

    for (int city : cities)
    {
        //Generowanie macierzy odległości
        auto distance_matrix = generate_distance_matrix(city);        
        vector<City> cities_points = distance_matrix.second;

        vector<double> timesBee;
        vector<double> distanceBee;
        vector<int> best_route_overall;
        double best_distance = std::numeric_limits<double>::max();

        for (int i = 0; i < bee_iteration - 1; i++)
        {
            auto start = chrono::high_resolution_clock::now();

            auto result = alg.abc_algorithm(distance_matrix.first, num_iterations, population_size);

            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            double time_sec = duration.count() / 1'000'000.0;
            timesBee.push_back(time_sec);
            distanceBee.push_back(result.second);

            if (result.second < best_distance) {
                best_distance = result.second;
                best_route_overall = result.first;
            }
        }

        save_results_bee(cities_points, best_route_overall, best_distance, bee_fileNames[iter]);

        double avgTime = std::accumulate(timesBee.begin(), timesBee.end(), 0.0) / bee_iteration;
        double avgDistance = std::accumulate(distanceBee.begin(), distanceBee.end(), 0.0) / bee_iteration;

        SaveFileInput resultInputBee;
        resultInputBee.cities = cities_points;
        resultInputBee.best_route = best_route_overall;
        resultInputBee.avaregeDitance = avgDistance;
        resultInputBee.avarageTime = avgTime;

        saveFileInputBee.push_back(resultInputBee);



        //Algorytm najbliższych somsiadów
        auto start = chrono::high_resolution_clock::now();

        auto result = nn_alg.nn_algorithm(city, distance_matrix.first);

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        double time_sec = duration.count() / 1'000'000.0;

        SaveFileInput resultInputNN;
        resultInputNN.cities = cities_points;
        resultInputNN.best_route = result.first;
        resultInputNN.avaregeDitance = result.second;
        resultInputNN.avarageTime = time_sec;
        saveFileInputNN.push_back(resultInputNN);

        save_results_nn(cities_points, result.first, result.second, nn_fileNames[iter]);

        ++iter;

    }

    save_multi_results(saveFileInputBee, mulitBeeFileName);
    save_multi_results(saveFileInputNN, mulitNNFileName);


    return 0;
}




//save_results_bee(cities_points, result.first, result.second);

//cout << "\n\nALG_ABC Czas dzialania dla " << city << " miast: " << times_bee.back() << "s";
//cout << "\nNajlepsza kolejnosc: " << endl;
//for (auto x : result.first)
//{
//    cout << x << " ";
//}
//cout << "\nOdleglos: " << result.second << endl;