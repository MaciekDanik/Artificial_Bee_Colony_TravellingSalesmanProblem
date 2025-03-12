#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

vector<vector<int>> generate_distance_matrix(int cities)
{
	vector<vector<int>> distance_matrix(cities, vector<int>(cities));

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, 100);

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
				int rnd = distrib(gen);// +1;
				distance_matrix[i][j] = rnd;
				distance_matrix[j][i] = rnd;
			}
		}
	}
	//cout << "Matrix size: " << distance_matrix.size() << endl;
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
