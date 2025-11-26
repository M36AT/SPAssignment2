#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct DistrictData {
    string name;
    double totalRainfall;
    int count;
};

// Trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void rainfallAnalysis() {
    ifstream inFile("Weather_Statistics.txt");
    
    if (!inFile) {
        cout << "Error: File not found!" << endl;
        return;
    }

    vector<DistrictData> districts;
    string line;

    getline(inFile, line); // Skip header

    while (getline(inFile, line)) {
        stringstream ss(line);
        string segment;
        string districtName;
        string rainfallStr;
        double rainfallVal;

        getline(ss, segment, ','); // State
        getline(ss, districtName, ','); // District
        districtName = trim(districtName); // Clean whitespace
        
        getline(ss, segment, ','); // Year
        getline(ss, segment, ','); // Temperature
        getline(ss, rainfallStr, ','); // Rainfall

        try {
            rainfallVal = stod(rainfallStr);
            if (rainfallVal < 0) rainfallVal = 0.0; // Validate
        } catch (...) {
            rainfallVal = 0.0;
        }

        bool found = false;
        for (size_t i = 0; i < districts.size(); i++) {
            if (districts[i].name == districtName) {
                districts[i].totalRainfall += rainfallVal;
                districts[i].count++;
                found = true;
                break;
            }
        }

        if (!found) {
            DistrictData newD;
            newD.name = districtName;
            newD.totalRainfall = rainfallVal;
            newD.count = 1;
            districts.push_back(newD);
        }
    }
    inFile.close();

    ofstream outFile("Rainfall_Report.txt");
    if (!outFile) {
        cout << "Error: Cannot create output file!" << endl;
        return;
    }
    
    cout << fixed << setprecision(2);
    outFile << fixed << setprecision(2);

    cout << "\n--- Rainfall Analysis ---\n";
    cout << left << setw(25) << "District" << "Average Rainfall (mm)" << endl;
    cout << string(50, '-') << "\n";
    
    outFile << "District Rainfall Analysis Report\n";
    outFile << string(50, '-') << "\n";

    for (size_t i = 0; i < districts.size(); i++) {
        double average = districts[i].totalRainfall / districts[i].count;

        outFile << "District: " << districts[i].name 
                << " | Total: " << districts[i].totalRainfall
                << " mm | Average: " << average << " mm\n";

        cout << left << setw(25) << districts[i].name << average;
        
        if (average > 1000) {
            cout << " [HIGH RAINFALL]";
        }
        cout << endl;
    }

    cout << "\nReport saved to Rainfall_Report.txt\n";
    outFile.close();
}

int main() {
    rainfallAnalysis();
    return 0;
}
