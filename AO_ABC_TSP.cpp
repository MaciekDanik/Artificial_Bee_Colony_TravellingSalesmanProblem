#include <iostream>
//#include "DistanceMatrix.cpp"
#include "distanceMatrix.h"

using namespace std;

int main()
{
    vector<vector<int>> distMatrix;

    int cities = 4;
    distMatrix = generate_distance_matrix(cities);

    printMatrix(distMatrix);
}