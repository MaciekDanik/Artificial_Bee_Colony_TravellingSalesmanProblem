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
        //total_dist += calculate_distance(solution[i], solution[i + 1], dist_matrix);
        total_dist += dist_matrix[solution[i]][solution[i + 1]];
    }

    //total_dist += calculate_distance(solution.back(), solution.front(), dist_matrix);
    total_dist += dist_matrix[solution.back()][solution.front()];

    return total_dist;
}

void ABC_Alg::employed_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, const int num_cities)
{
    //#pragma omp parallel //for private(gen, distrib) //Tutaj by³o zakomentowane
    for (int i = 0; i < solutions.size(); ++i)
    {
        mt19937 gen(random_device{}());  // jeden generator na w¹tek
        uniform_int_distribution<> distrib(0, num_cities - 1);

        vector<int> new_solution = solutions[i];
        int rnd_pos1 = distrib(gen);
        int rnd_pos2 = distrib(gen);
        swap(new_solution[rnd_pos1], new_solution[rnd_pos2]);

        int new_fitness = calculate_fitness(new_solution, dist_matrix);

        if (new_fitness < fitness_values[i])
        {
            solutions[i] = new_solution;
            fitness_values[i] = new_fitness;
        }
    }


    //random_device rd;
    //mt19937 gen(rd());
    //uniform_int_distribution<> distrib(0, num_cities - 1);
    ////#pragma omp parallel for
    //for (int i = 0; i < solutions.size(); ++i)
    //{
    //    int rnd_pos1 = distrib(gen);
    //    int rnd_pos2 = distrib(gen);
    //    //swap two randolmy selected cities to create a new solution
    //    swap(solutions[i][rnd_pos1], solutions[i][rnd_pos2]);

    //    int new_fitness = calculate_fitness(solutions[i], dist_matrix);

    //    if (new_fitness < fitness_values[i])
    //    {
    //        //if the new fitness is better (shorter distance) 
    //        // replace the solution with new_solution
    //        fitness_values[i] = new_fitness;
    //    }
    //    else
    //    {
    //        swap(solutions[i][rnd_pos1], solutions[i][rnd_pos2]);
    //    }
    //}
}

void ABC_Alg::onlooker_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_cities, vector<int>& not_improved)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> city_dist(0, num_cities - 1);

    //#pragma omp parallel for shared(gen, city_dist)
    for (int i = 0; i < solutions.size(); ++i) 
    {
        // Oblicz prawdopodobieñstwo wyboru rozwi¹zania
        vector<double> probabilities(fitness_values.size());
        double sum_fitness = 0.0;

            for (double fitness : fitness_values) 
            {
                sum_fitness += fitness;
            }

        for (int j = 0; j < fitness_values.size(); ++j) 
        {
            probabilities[j] = fitness_values[j] / sum_fitness;
        }

        discrete_distribution<int> distrib(probabilities.begin(), probabilities.end());
        int rnd_index = distrib(gen);

        int pos1 = city_dist(gen);
        int pos2 = city_dist(gen);
        if (pos1 > pos2) 
        {
            swap(pos1, pos2);
        }

        vector<int> new_solution = solutions[rnd_index];
        reverse(new_solution.begin() + pos1, new_solution.begin() + pos2 + 1);

        double new_fitness = calculate_fitness(new_solution, dist_matrix);

        if (new_fitness < fitness_values[rnd_index]) 
        {
            fitness_values[rnd_index] = new_fitness;
            solutions[rnd_index] = new_solution;
            not_improved[rnd_index] = 0; // Reset trial count
        }
        else 
        {
            not_improved[rnd_index]++;
        }
    }
}

void ABC_Alg::scout_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_iterations, int num_cities, vector<int>& not_improved)
{
    auto worst_idx = max_element(fitness_values.begin(), fitness_values.end());
    int worst_solution_idx = distance(fitness_values.begin(), worst_idx);

    if (num_iterations % 10 == 0)
    {
        solutions[worst_solution_idx] = generate_initial_solution(num_cities);
        fitness_values[worst_solution_idx] = calculate_fitness(solutions[worst_solution_idx], dist_matrix);
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

    auto min_iterator = min_element(fitness_values.begin(), fitness_values.end());
    int index = distance(fitness_values.begin(), min_iterator);

    int best_fitness = fitness_values[index];
    vector<int> best_solution = solutions[index];

    //#pragma omp parallel for shared(dist_matrix, num_cities, solutions, fitness_values, not_improved)
    for (int iteration = 0; iteration < num_iterations; ++iteration)
    {

        //Employed bee phase
        //auto start_employed = chrono::high_resolution_clock::now();
        employed_bee_phase(solutions, fitness_values, dist_matrix, num_cities);
        //auto stop_employed = chrono::high_resolution_clock::now();
        //auto duration_employed = chrono::duration_cast<chrono::microseconds>(stop_employed - start_employed);
        //cout << "\nEmployed bee phase: " << (duration_employed.count() / 1000000.0)<< "s\n";

        //Onlooker bee phase
        //auto start_onlooker = chrono::high_resolution_clock::now();
        onlooker_bee_phase(solutions, fitness_values, dist_matrix, num_cities, not_improved);
        //auto stop_onlooker = chrono::high_resolution_clock::now();
        //auto duration_onlooker = chrono::duration_cast<chrono::microseconds>(stop_onlooker - start_onlooker);
        //cout << "\nOnlooker bee phase: " << (duration_onlooker.count() / 1000000.0) << "s\n";

        //Scout bee phase
        //auto start_scout = chrono::high_resolution_clock::now();
        scout_bee_phase(solutions, fitness_values, dist_matrix, iteration, num_cities, not_improved);
        //auto stop_scout = chrono::high_resolution_clock::now();
        //auto duration_scout = chrono::duration_cast<chrono::microseconds>(stop_scout - start_scout);
        //cout << "\nScout bee phase: " << (duration_scout.count() / 1000000.0) << "s\n";


        for (const auto& solution : solutions)
        {
            int fitness = calculate_fitness(solution, dist_matrix);
            if (fitness < best_fitness)
            {
                //cout << "\nOld fitness: " << best_fitness << "\n";
                best_solution = solution;
                best_fitness = fitness;
                //cout << "New fitness: " << best_fitness << "\n";
            }
        }
    }

    return {best_solution, best_fitness};
}

