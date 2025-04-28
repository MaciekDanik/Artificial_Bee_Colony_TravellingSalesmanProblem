#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <iomanip>
#include <omp.h>

using namespace std;

class ABC_Alg
{
public:
	/*
	* Calculates the distance betwen two cities.
	* Parameters:
	* city1 -> index of first city
	* city2 -> index of second city
	* dist_matrix -> the distance matrix.
	* Returns: the distance between the two cities.
	*/
	int calculate_distance(const int city1,const int city2, const vector<vector<int>> dist_matrix);
	/*
	* Generates random initial solution.
	*
	* Parameters: num_cities -> the number of cities.
	*
	* Returns: A vector with the random solution
	*/
	vector<int> generate_initial_solution(const int num_cities);
	/*
	* calculates fitness (total distance) for a given solution.
	* Parameters:
	* solution -> a vector with a solution
	* dist_matrix -> distance matrix
	* Returns:
	* The fitness of a solution
	*/
	int calculate_fitness(const vector<int>& solution, const vector<vector<int>>& dist_matrix);
	/*
	* Employed bee phase 
	* solutions -> a vector with all solutions
	* dist_matris -> distance matrix
	* num_cities -> number of cities
	*/
	void employed_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, const int num_cities);
	/*
	* onlooker bee phase
	* solutions -> a vector with all solutions
	* fitness_values -> fitness values of solutions
	* dist_matris -> distance matrix
	* num_cities -> number of cities
	*/
	void onlooker_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_cities, vector<int>& not_improved);
	/*
	* scout bee phase
	* solutions -> a vector with all solutions
	* fitness_values -> fitness values of solutions
	* num_iterations -> number of current iteration
	* num_cities -> number of cities 
	*/

	//void scout_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_iterations, int num_cities, vector<int>& not_improved);
	//void scout_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_cities, int num_iterations);
	void scout_bee_phase(vector<vector<int>>& solutions, vector<double>& fitness_values, const vector<vector<int>>& dist_matrix, int num_cities, vector<int>& not_improved, int num_iterations);
	/*
	* ABC algorithm
	* 
	* dist_matrix -> distance matrix
	* num_iterations -> number of iterations
	* pop_size -> population size
	* 
	* returns: best solution and its fitness
	*/
	pair<vector<int>, int> abc_algorithm(const vector<vector<int>>& dist_matrix, int num_iterations, int pop_size);
};

