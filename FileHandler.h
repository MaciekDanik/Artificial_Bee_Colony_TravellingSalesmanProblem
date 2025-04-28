#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


class SaveFileInput {
public:
    vector<City> cities;
    vector<int> best_route;
    double avaregeDitance;
    double avarageTime;

    int citiesCount() const {
        return static_cast<int>(cities.size());
    }
};


void save_results_bee(const vector<City>& cities, const vector<int>& best_route, double best_distance, string filename) 
{
    string folderPath = "Bee_results/";
    string filePath = folderPath + filename;

    if (!fs::exists(folderPath)) {
        if (!fs::create_directory(folderPath)) {
            cerr << "Error creating directory!" << endl;
            return;
        }
    }

    ofstream file(filePath);

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

void save_results_nn(const vector<City>& cities, const vector<int>& best_route, double best_distance, string filename) 
{
    string folderPath = "NN_results/";
    string filePath = folderPath + filename;

    if (!fs::exists(folderPath)) {
        if (!fs::create_directory(folderPath)) {
            cerr << "Error creating directory!" << endl;
            return;
        }
    }

    ofstream file(filePath);

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


const string mulitBeeFileName = "results_bee_multi.txt";
const string mulitNNFileName = "results_nn_multi.txt";

//funkcja zapisuje do pliku dane o czasie wykonywania algorytmu, roz³o¿eniu miast, trasie dla wielu generacji miast uwzgledniaj¹c ró¿n¹ liczbê miast
void save_multi_results(const std::vector<SaveFileInput>& results, const string& fileName) {
    std::ofstream file(fileName);

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    for (size_t i = 0; i < results.size(); ++i) {
        const SaveFileInput& result = results[i];
        file << "=== Result " << i + 1 << " ===\n";

        file << "Cities:\n";
        for (size_t j = 0; j < result.cities.size(); ++j) {
            file << j << " " << result.cities[j].x << " " << result.cities[j].y << "\n";
        }

        file << "\nBest Route:\n";
        for (int city : result.best_route) {
            file << city << " ";
        }

        file << "\nAverage Distance:\n" << result.avaregeDitance;
        file << "\nAverage Time:\n" << result.avarageTime;
        file << "\nNumber of Cities:\n" << result.citiesCount();
        file << "\n\n----------------------------------\n\n";
    }

    file.close();
}
