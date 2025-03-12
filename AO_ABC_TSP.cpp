#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <chrono>
#include <iomanip>

#include "distanceMatrix.h"
#include "ABC_Alg.h"

using namespace std;

int main()
{
    vector<vector<int>> distMatrix;
    ABC_Alg alg;

    int cities = 4;
    distMatrix = generate_distance_matrix(cities);

    vector<int> sol(cities);
    sol = alg.generate_initial_solution(cities);
    for (const auto x : sol)
    {
        cout << x << endl;
    }
    printMatrix(distMatrix);




}