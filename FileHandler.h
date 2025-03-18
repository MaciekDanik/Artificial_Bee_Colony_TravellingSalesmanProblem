#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

void save_results(const vector<City>& cities, const vector<int>& best_route, double best_distance) {
    ofstream file("results.txt");

    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    file << "Cities:\n";
    for (size_t i = 0; i < cities.size(); ++i) {
        file << i << " " << cities[i].x << " " << cities[i].y << "\n";
    }

    file << "\nBest Route:\n";
    for (int city : best_route) {
        file << city << " ";
    }

    file << "\nTotal Distance:\n" << best_distance << "\n";
    file.close();
}
