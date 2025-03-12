#include "ABC_Alg.h"

int ABC_Alg::calculate_distance(const int city1, const int city2, const vector<vector<int>> dist_matrix)
{
	return dist_matrix[city1][city2];
}

vector<int> ABC_Alg::generate_initial_solution(const int num_cities)
{
    vector<int> initial_solution(num_cities);
    iota(initial_solution.begin(), initial_solution.end(), 0);

    random_device rd;
    mt19937 gen(rd());
    shuffle(initial_solution.begin(), initial_solution.end(), gen);

    return initial_solution;
}
