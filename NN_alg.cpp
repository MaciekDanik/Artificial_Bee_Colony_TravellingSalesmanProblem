#include "NN_alg.h"

vector<int> NN_alg::nearest_neighbor(int start_city, int num_cities, const vector<vector<int>>& dist_matrix)
{
    vector<int> solution;
    vector<bool> visited(num_cities, false);
    int current_city = start_city;

    solution.push_back(current_city);
    visited[current_city] = true;

    for (int i = 1; i < num_cities; ++i)
    {
        int nearest_neighbor = -1;
        int min_distance = numeric_limits<int>::max();

        for (int next_city = 0; next_city < num_cities; ++next_city)
        {
            if (!visited[next_city])
            {
                if (dist_matrix[current_city][next_city] < min_distance)
                {
                    min_distance = dist_matrix[current_city][next_city];
                    nearest_neighbor = next_city;
                }
            }
        }

        if (nearest_neighbor != -1)
        {
            solution.push_back(nearest_neighbor);
            visited[nearest_neighbor] = true;
            current_city = nearest_neighbor;
        }
        else
        {
            break;
        }
    }

    solution.push_back(start_city);
    
    return solution;
}

int NN_alg::calculate_fitness(const vector<int>& solution, const vector<vector<int>>& dist_matrix)
{
    int total_dist = 0;

    for (int i = 0; i < solution.size() - 1; ++i)
    {
        total_dist += dist_matrix[solution[i]][solution[i + 1]];
    }

    total_dist += dist_matrix[solution.back()][solution.front()];

    return total_dist;
}

pair<vector<int>, int> NN_alg::nn_algorithm(int num_cities, const vector<vector<int>>& dist_matrix)
{
    vector<vector<int>> solutions(num_cities);
    vector<int> fitness_values(num_cities);
    int best_fitness = numeric_limits<int>::max();
    vector<int> best_solution;

    #pragma omp parallel for shared(dist_matrix, num_cities, solutions, fitness_values)
    for (int start_city = 0; start_city < num_cities; ++start_city)
    {
        solutions[start_city] = nearest_neighbor(start_city, num_cities, dist_matrix);
        fitness_values[start_city] = calculate_fitness(solutions[start_city], dist_matrix);
    }

    for (int i = 0; i < num_cities; ++i)
    {
        if (fitness_values[i] < best_fitness)
        {
            best_solution = solutions[i];
            best_fitness = fitness_values[i];
        }
    }

    return {best_solution, best_fitness};
}
