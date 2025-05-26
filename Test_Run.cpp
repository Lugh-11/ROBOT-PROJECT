// *********************************************************
// Program: TC3L_G15_main.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC3L
// Tutorial Class: TT9L, T11L
// Trimester: 2410
// Member_1: 
// Member_2: 
// Member_3: 
// Member_4: 
// *********************************************************
// Task Distribution
// Member_1: 
// Member_2: 
// Member_3: 
// Member_4: 
// *********************************************************
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Robot {
    string id;
    int x, y;
};

void processFile(const string& inputFile, const string& outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);
    if (!fin) {
        cout << "Cannot open " << inputFile << endl;
        return;
    }
    if (!fout) {
        cout << "Cannot open " << outputFile << " for writing\n";
        return;
    }

    string line;
    int rows = 0, cols = 0, turns = 0, robotCount = 0;
    vector<Robot> robots;

    // Read grid size
    getline(fin, line);
    istringstream iss1(line.substr(line.find(":") + 1));
    iss1 >> rows >> cols;

    // Read turns
    getline(fin, line);
    istringstream iss2(line.substr(line.find(":") + 1));
    iss2 >> turns;

    // Read robot count
    getline(fin, line);
    istringstream iss3(line.substr(line.find(":") + 1));
    iss3 >> robotCount;

    // Read robots
    for (int i = 0; i < robotCount; ++i) {
        getline(fin, line);
        istringstream iss(line);
        string type, id, xstr, ystr;
        iss >> type >> id >> xstr >> ystr;
        int x, y;
        if (xstr == "random" || ystr == "random") {
            x = 0; y = 0;
        } else {
            x = stoi(xstr);
            y = stoi(ystr);
        }
        robots.push_back({id.substr(0, 4), x, y});
    }

    // Prepare grid
    vector<vector<string>> grid(rows, vector<string>(cols, "    "));
    for (const auto& r : robots) {
        if (r.x >= 0 && r.x < rows && r.y >= 0 && r.y < cols)
            grid[r.x][r.y] = r.id;
    }

    // Lambda to print to any stream
    auto printGrid = [&](ostream& os) {
        os << "     ";
        for (int j = 0; j < cols; ++j)
            os << setw(4) << j;
        os << endl;
        for (int i = 0; i < rows; ++i) {
            os << "    +";
            for (int j = 0; j < cols; ++j) os << "----+";
            os << endl;
            os << setw(3) << i << " |";
            for (int j = 0; j < cols; ++j)
                os << setw(4) << grid[i][j] << "|";
            os << endl;
        }
        os << "    +";
        for (int j = 0; j < cols; ++j) os << "----+";
        os << endl;
    };

    // Print to terminal and file
    cout << "=== " << inputFile << " ===" << endl;
    printGrid(cout);
    printGrid(fout);
}

int main() {
    for (int i = 1; i <= 6; ++i) {
        string inputFile = "fileInput" + to_string(i) + ".txt";
        string outputFile = "fileOutput" + to_string(i) + ".txt";
        processFile(inputFile, outputFile);
    }
    return 0;
}


