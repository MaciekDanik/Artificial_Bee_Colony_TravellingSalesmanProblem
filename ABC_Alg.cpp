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

int ABC_Alg::calculate_fitness(const vector<int>& solution, const vector<vector<int>>& dist_matrix)
{
    int total_dist = 0;

    for (int i = 0; i < solution.size() - 1; ++i)
    {
        total_dist += calculate_distance(solution[i], solution[i + 1], dist_matrix);
    }

    total_dist += calculate_distance(solution.back(), solution.front(), dist_matrix);

    return total_dist;
}

void ABC_Alg::employed_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, const int num_cities)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, num_cities - 1);
    for (int i = 0; i<solutions.size();++i)
    {
        int rnd_pos1 = distrib(gen);
        int rnd_pos2 = distrib(gen);
        //swap two randolmy selected cities to create a new solution
        swap(solutions[i][rnd_pos1], solutions[i][rnd_pos2]);

        int new_fitness = calculate_fitness(solutions[i], dist_matrix);

        if (new_fitness < fitness_values[i])
        {
            //if the new fitness is better (shorter distance) 
            // replace the solution with new_solution
            fitness_values[i] = new_fitness;
        }
        else
        {
            swap(solutions[i][rnd_pos1], solutions[i][rnd_pos2]);
        }
    }


    /*
    for (auto& solution : solutions)
    {
        //create a new vector the same size as solution to store new solution
        vector<int> new_solution = solution;
        int rnd_pos1 = distrib(gen);
        int rnd_pos2 = distrib(gen);
        //swap two randolmy selected cities to create a new solution
        swap(new_solution[rnd_pos1], new_solution[rnd_pos2]);

        int new_fitness = calculate_fitness(new_solution, dist_matrix);

        if (new_fitness < calculate_fitness(solution, dist_matrix))
        {
            //if the new fitness is better (shorter distance) 
            // replace the solution with new_solution
            solution = new_solution;
        }
    }*/
}

void ABC_Alg::onlooker_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_cities)
{
    random_device rd;
    mt19937 gen(rd());
    discrete_distribution<int> distrib(fitness_values.begin(), fitness_values.end());

    for (int i = 0; i < solutions.size(); ++i)
    {
        int rnd_index = distrib(gen);
        two_opt(solutions[rnd_index], solutions[0].size());

        int new_fitness = calculate_fitness(solutions[rnd_index], dist_matrix);
        if (new_fitness < fitness_values[rnd_index])
        {
            fitness_values[rnd_index] = new_fitness;
        }
    }


    /*
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distrib_real(0.0, 1.0);
    uniform_int_distribution<> distrib(0, num_cities - 1);

    double fitness_sum = accumulate(fitness_values.begin(), fitness_values.end(), 0.0);
    vector<double> probabilities(fitness_values.size());

    for (int i = 0; i < fitness_values.size(); ++i)
    {
        probabilities[i] = fitness_values[i] / fitness_sum;
    }

    for (int i = 0; i < solutions.size(); ++i)
    {
        double rnd_value = distrib_real(gen);
        double probabilities_sum = 0.0;
        int solution_index = 0;

        for (int j = 0; j < probabilities.size(); ++j)
        {
            probabilities_sum += probabilities[i];
            if (rnd_value <= probabilities_sum)
            {
                solution_index = j;
                break;
            }
        }

        vector<int> new_solution = solutions[solution_index];
        
        int rnd_pos1 = distrib(gen);
        int rnd_pos2 = distrib(gen);
        swap(new_solution[rnd_pos1], new_solution[rnd_pos2]);

        int new_fitness = calculate_fitness(new_solution, dist_matrix);

        if (new_fitness < calculate_fitness(solutions[i], dist_matrix))
        {
            // if better than curent solution at solution_index, replace
            solutions[solution_index] = new_solution;
        }
    }*/
}

void ABC_Alg::scout_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, vector<int> not_improved, int num_cities)
{
    for (int i = 0; i < solutions.size(); ++i)
    {
        if (not_improved[i] > 10)
        {
            solutions[i] = generate_initial_solution(num_cities);
            fitness_values[i] = calculate_fitness(solutions[i], dist_matrix);
            not_improved[i] = 0;
        }
    }
}

pair<vector<int>, int> ABC_Alg::abc_algorithm(const vector<vector<int>>& dist_matrix, int num_iterations, int pop_size)
{
    int num_cities = dist_matrix.size();
    vector<vector<int>> solutions(pop_size);
    vector<double> fitness_values(pop_size);
    vector<int> not_improved(pop_size, 0);

    for (int i = 0; i < pop_size; ++i) {
        solutions[i] = generate_initial_solution(num_cities);
        fitness_values[i] = calculate_fitness(solutions[i], dist_matrix);
    }

    vector<int> best_solution = solutions[0];
    int best_fitness = fitness_values[0];
    //#pragma omp parallel for
    for (int iteration = 0; iteration < num_iterations; ++iteration)
    {
        employed_bee_phase(solutions, fitness_values, dist_matrix, num_cities);
        //#pragma omp parallel for
        //for (int j = 0; j < pop_size; ++j)
        //{
        //    fitness_values[j] = calculate_fitness(solutions[j], dist_matrix);
        //}

        onlooker_bee_phase(solutions, fitness_values, dist_matrix, num_cities);
        scout_bee_phase(solutions, fitness_values, dist_matrix, not_improved, num_cities);

        for (int i = 0; i < pop_size; ++i)
        {
            if (fitness_values[i] < best_fitness)
            {
                best_solution = solutions[i];
                best_fitness = fitness_values[i];
                not_improved[i] = 0;
            }
            else
            {
                not_improved[i]++;
            }
        }

        //for (const auto& solution : solutions)
        //{
        //    int fitness = calculate_fitness(solution, dist_matrix);
        //    if (fitness < best_fitness)
        //    {
        //        best_solution = solution;
        //        best_fitness = fitness;
        //    }
        //}
    }

    return {best_solution, best_fitness};
}

void ABC_Alg::two_opt(vector<int>& solution, int num_cities)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, num_cities - 1);

    int i = distrib(gen);
    int k = distrib(gen);
    if (i > k) swap(i, k);

    reverse(solution.begin() + i, solution.begin() + k + 1);
}

