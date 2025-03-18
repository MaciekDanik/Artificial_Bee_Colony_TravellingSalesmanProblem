#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include "City.h"

using namespace std;

int random_number_generator(int from = 1,int to = 100)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(from, to);

	return distrib(gen);
}

vector<vector<int>> generate_distance_matrix2(int cities)
{
	vector<vector<int>> distance_matrix(cities, vector<int>(cities));

	for (int i = 0; i < cities; ++i)
	{
		for (int j = i; j < cities; ++j)
		{
			if (i == j)
			{
				distance_matrix[i][j] = 0;
			}
			else
			{
				int rnd = random_number_generator();// +1;
				distance_matrix[i][j] = rnd;
				distance_matrix[j][i] = rnd;
			}
		}
	}
	//cout << "Matrix size: " << distance_matrix.size() << endl;
	return distance_matrix;
}

vector<vector<int>> generate_distance_matrix(int cities) {

	vector<vector<int>> distance_matrix(cities, vector<int>(cities, 0));

	for (int i = 0; i < cities; ++i) {
		for (int j = i + 1; j < cities; ++j) {
			int dist = random_number_generator();
			distance_matrix[i][j] = distance_matrix[j][i] = dist;
		}
	}
	return distance_matrix;
}

void printMatrix(vector<vector<int>> distMatrix)
{
	int mSize = distMatrix.size();
	for (int i = 0; i < mSize; ++i)
	{
		for (int j = 0; j < mSize; ++j)
		{
			cout << distMatrix[i][j] << " ";
		}
		cout << "\n";
	}
}

vector<City> genereate_cities_points(int num_cities)
{
	vector<City> cities(num_cities);

	for (int i = 0; i < cities.size(); i++)
	{
		cities[i].x = random_number_generator();
		cities[i].y = random_number_generator();
	}

	return cities;
}

int calculate_distance(const City& city1, const City& city2) {
	return round(sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2)));
}

vector<vector<int>> generate_distance_matrix_by_points(const vector<City>& cities) {
	int num_cities = cities.size();
	vector<vector<int>> distance_matrix(num_cities, vector<int>(num_cities, 0));

	for (int i = 0; i < num_cities; ++i) {
		for (int j = i + 1; j < num_cities; ++j) {
			int distance = calculate_distance(cities[i], cities[j]);
			distance_matrix[i][j] = distance;
			distance_matrix[j][i] = distance;
		}
	}

	return distance_matrix;
}

pair<vector<vector<int>>, vector<City>> generate_distance_matrix3(int num_cities)
{
	vector<City> cities = genereate_cities_points(num_cities);
	vector<vector<int>> distance_matrix = generate_distance_matrix_by_points(cities);

	return { distance_matrix, cities };
}