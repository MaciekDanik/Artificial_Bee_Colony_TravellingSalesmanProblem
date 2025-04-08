#pragma once
#include <vector>
#include <limits>
#include <numeric>
#include <omp.h>
#include <algorithm>
#include <iostream>

using namespace std;

class NN_alg
{
public:
	vector<int> nearest_neighbor(int start_city, int num_cities, const vector<vector<int>>& dist_matrix);
	int calculate_fitness(const vector<int>& solution, const vector<vector<int>>& dist_matrix);
	pair<vector<int>, int> nn_algorithm(int num_citeis, const vector<vector<int>>& dist_matrix);
};

