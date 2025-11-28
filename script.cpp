#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct DistrictData {
    string name;
    double totalRainfall;
    int count;
};

struct DistrictData {
    string name;
    double totalRainfall;
    int count;
};

struct WeatherData {
    string district;
    int year;
    double avgTemp;
};

// Trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void temperatureTrends() {
    ifstream infile("Weather_Statistics.txt");
    ofstream highfile("High_Temp.txt");
    ofstream lowfile("Low_Temp.txt");
    
    string state, district;
    int year;
    double temp, rainfall, humidity;
    
    // Headers
    highfile << "State District Year Temperature Rainfall Humidity\n";
    lowfile << "State District Year Temperature Rainfall Humidity\n";
    
    string header;
    getline(infile, header);
    
    while(infile >> state >> district >> year >> temp >> rainfall >> humidity) {
        if (temp > 30.0) {
            highfile << state << " " << district << " " << year << " " << temp << " " << rainfall << " " << humidity << "\n"; }
		else{
			lowfile << state << " " << district << " " << year << " " << temp << " " << rainfall << humidity << "\n";
			
		}
	}
	
	infile.close();
	highfile.close();
	lowfile.close();
	
	cout << "Temperature analysis done! Check High_Temp.txt and Low_Temp.txt.\n";
	
	return 0;
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

void climateChangeImpact() {
    ifstream file("Weather_Statistics.txt");
    ofstream report("ClimateImpact_Report.txt");

    if (!file.is_open()) {
        cout << "Error: Cannot open Weather_Statistics.txt\n";
        return;
    }

    if (!report.is_open()) {
        cout << "Error: Cannot create ClimateImpact_Report.txt\n";
        return;
    }

    WeatherData data[500];  // Beginner friendly fixed-size array
    int count = 0;

    string line;
    getline(file, line); // Skip header

    // Read data from file
    while (getline(file, line)) {
        stringstream ss(line);
        string state, district, yearStr, tempRange, rainfall, humidity;

        getline(ss, state, ',');
        getline(ss, district, ',');
        getline(ss, yearStr, ',');
        getline(ss, tempRange, ',');
        getline(ss, rainfall, ',');
        getline(ss, humidity, ',');

        // Convert year
        int year = stoi(yearStr);

        // Extract temperature range (e.g. "25-30")
        int dashPos = tempRange.find('-');
        double minT = stod(tempRange.substr(0, dashPos));
        double maxT = stod(tempRange.substr(dashPos + 1));

        double avgT = (minT + maxT) / 2.0;

        // Store into array
        data[count].district = district;
        data[count].year = year;
        data[count].avgTemp = avgT;

        count++;
    }

    // Write report header
    report << "===== CLIMATE CHANGE IMPACT REPORT =====\n\n";

    // Compare each pair of records
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {

            // Only compare same district
            if (data[i].district == data[j].district) {

                // Compare year-to-year temperature
                double diff = data[j].avgTemp - data[i].avgTemp;

                // Check if change is 2°C or more
                if (abs(diff) >= 2.0) {
                    report << "District: " << data[i].district << "\n";
                    report << "From Year " << data[i].year 
                           << " to " << data[j].year << "\n";
                    report << "Temperature change: " << diff << "°C\n";
                    report << "---------------------------------\n";
                }
            }
        }
    }

    cout << "Climate Change Impact report created: ClimateImpact_Report.txt\n";

    file.close();
    report.close();
}

int main() {
    rainfallAnalysis();
    return 0;
}

