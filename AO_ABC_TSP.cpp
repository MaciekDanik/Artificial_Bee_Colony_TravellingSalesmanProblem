#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include "distanceMatrix.h"
#include "ABC_Alg.h"
#include "FileHandler.h"
#include "NN_alg.h"


using namespace std;

int main()
{
    ABC_Alg alg;
    NN_alg nn_alg;
    //vector<int> cities = { 10, 50, 100, 250, 500, 1000, 5000 };
    vector<int> cities = { 100 };
    vector<double> times;
    vector<double> times_nn;

    int num_iterations = 20000;
    int population_size = 50;

    for (int city : cities)
    {
        //Generowanie macierzy odległości
        auto distance_matrix = generate_distance_matrix(city);        
        //printMatrix(distance_matrix.first);
        vector<City> cities_points = distance_matrix.second;




        //Algorytm sztucznego roju pszczół
        auto start = chrono::high_resolution_clock::now();
        auto result = alg.abc_algorithm(distance_matrix.first, num_iterations, population_size);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        times.push_back(duration.count() / 1000000.0);
        cout << "\n\nALG_ABC Czas dzialania dla " << city << " miast: " << times.back() << "s";
        cout << "\nNajlepsza kolejnosc: " << endl;
        for (auto x : result.first)
        {
            cout << x << " ";
        }
        cout << "\nOdleglos: " << result.second << endl;
        //zapis wyników do pliku
        save_results(cities_points, result.first, result.second);









        //Algorytm najbliższych somsiadów

        //auto start_nn = chrono::high_resolution_clock::now();
        //auto result_nn = nn_alg.nn_algorithm(city, distance_matrix.first);
        //auto stop_nn = chrono::high_resolution_clock::now();

        //auto duration_nn = chrono::duration_cast<chrono::microseconds>(stop_nn - start_nn);
        //times_nn.push_back(duration_nn.count() / 1000000.0);

        //cout << "\n\nALG_NN Czas dzialania dla " << city << " miast: " << times_nn.back() << "s";
        //cout << "\nNajlepsza kolejnosc: " << endl;
        //for (auto x : result_nn.first)
        //{
        //    cout << x << " ";
        //}
        //cout << "\nOdleglos: " << result_nn.second << endl;
        //
        //save_results(cities_points, result_nn.first, result_nn.second);
    }


    return 0;
}