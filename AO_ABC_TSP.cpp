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


using namespace std;

int main()
{
    ABC_Alg alg;
    //vector<int> cities = { 10, 50, 100, 250, 500, 1000, 5000 };
    vector<int> cities = { 30 };
    vector<double> times;

    int num_iterations = 1500;
    int population_size = 50;

    for (int city : cities)
    {
        auto distance_matrix = generate_distance_matrix(city);        
        printMatrix(distance_matrix.first);

        vector<City> cities_points = distance_matrix.second;

        auto start = chrono::high_resolution_clock::now();
        auto result = alg.abc_algorithm(distance_matrix.first, num_iterations, population_size);
        auto stop = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        times.push_back(duration.count() / 1000000.0);

        cout << "\n\nCzas dzialania dla " << city << " miast: " << times.back() << "s";

        cout << "\nNajlepsza kolejnosc: " << endl;
        for (auto x : result.first)
        {
            cout << x << " ";
        }
        cout << "\nOdleglos: " << result.second << endl;

        save_results(cities_points, result.first, result.second);
    }


    return 0;
}