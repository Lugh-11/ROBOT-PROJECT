// *********************************************************
// Program: Robot Battle Simulation
// Course: CCP6124 OOPDS
// Lecture Class: TC3L
// Tutorial Class: 
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
#include <random>
#include <set>
#include <queue>
#include <ctime>

using namespace std;

// Global random generator
// mt19937 gen(static_cast<unsigned int>(time(0)));

// Abstract base class
class Robot {
protected:
    string type, id;
    int x, y;
public:
    int lives;
    Robot(string type, string id, int x, int y, int lives = 3) : type(type), id(id), x(x), y(y), lives(lives) {}
    virtual ~Robot() {}
    virtual string getType() const = 0;
    virtual string getId() const = 0;
    virtual int getX() const = 0;
    virtual int getY() const = 0;
};  // End of Robot class

class MovingRobot : virtual public Robot {
public:
    MovingRobot(const string& type, const string& id, int x, int y, int lives = 3)
        : Robot(type, id, x, y, lives) {}
    virtual void move(int dx, int dy, int rows, int cols) = 0;
};  // End of MovingRobot class

class ShootingRobot : virtual public Robot {
public:
    ShootingRobot(const string& type, const string& id, int x, int y, int lives = 3)
        : Robot(type, id, x, y, lives) {}
    virtual bool fire(int dx, int dy, vector<Robot*>& robots, int rows, int cols, ostream& os) = 0;
};  // End of ShootingRobot class

class SeeingRobot : virtual public Robot {
public:
    SeeingRobot(const string& type, const string& id, int x, int y, int lives = 3)
        : Robot(type, id, x, y, lives) {}
    virtual void look(const vector<vector<string>>& grid, int rows, int cols, ostream& os, int dx = 0, int dy = 0) const = 0;
};  // End of SeeingRobot class

class ThinkingRobot : virtual public Robot {
public:
    ThinkingRobot(const string& type, const string& id, int x, int y, int lives = 3)
        : Robot(type, id, x, y, lives) {}
    virtual void think() = 0;
};  // End of ThinkingRobot class

// GenericRobot with upgrade system and lives
class GenericRobot : public MovingRobot, public ShootingRobot, public SeeingRobot, public ThinkingRobot {
    int shells;
    int hideCount, jumpCount, scoutCount, trackerCount, killCount;
    bool hasHide, hasJump, hasLongShot, hasSemiAuto, hasThirtyShot, hasScout, hasTrack, hasSniper;
    set<string> upgrades; // "moving", "shooting", "seeing"
    bool hidden;

public:
    GenericRobot(string type, string id, int x, int y, int lives = 3)
        : Robot(type, id, x, y, lives), 
        MovingRobot(type, id, x, y, lives), 
        ShootingRobot(type, id, x, y, lives),
        SeeingRobot(type, id, x, y, lives), 
        ThinkingRobot(type, id, x, y, lives),
        shells(10), hideCount(3), jumpCount(3), scoutCount(3), trackerCount(3), killCount(0),
        hasHide(false), hasJump(false), hasLongShot(false), hasSemiAuto(false), hasLaser(false),
        hasThirtyShot(false), hasScout(false), hasTrack(false), hasSniper(false), hidden(false) {}

    bool hasLaser;

    void upgradeRandomChoice(ostream& os) {
        if (upgrades.size() >= 3) {
            os << "Robot " << id << " has reached max upgrades.\n";
            return;
        }

        // Collect all possible upgrades from areas not yet upgraded
        struct UpgradeOption {
            string area;
            string upgrade;
        };

        vector<UpgradeOption> possibleUpgrades;

        if (upgrades.count("moving") == 0) {
            if (!hasJump) possibleUpgrades.push_back({"moving", "jump"});
            if (!hasHide) possibleUpgrades.push_back({"moving", "hide"});
        }

        if (upgrades.count("shooting") == 0) {
            if (!hasLongShot) possibleUpgrades.push_back({"shooting", "longShot"});
            if (!hasSemiAuto) possibleUpgrades.push_back({"shooting", "semiAuto"});
            if (!hasThirtyShot) possibleUpgrades.push_back({"shooting", "thirtyShot"});
            if (!hasLaser) possibleUpgrades.push_back({"shooting", "laser"});
            if (!hasSniper) possibleUpgrades.push_back({"shooting", "sniper"}); 
        }

        if (upgrades.count("seeing") == 0) {
            if (!hasScout) possibleUpgrades.push_back({"seeing", "scout"});
            if (!hasTrack) possibleUpgrades.push_back({"seeing", "track"});
        }

        if (possibleUpgrades.empty()) {
            os << "Robot " << id << " has no available upgrades left.\n";
            return;
        }

        // Pick a random upgrade from possible options
        int idx = rand() % possibleUpgrades.size();
        UpgradeOption choice = possibleUpgrades[idx];

        // Apply the chosen upgrade
        if (choice.area == "moving") {
            if (choice.upgrade == "jump") {
                hasJump = true;
                os << "Robot " << id << " upgrades Moving ability: Jump enabled.\n";
            } else if (choice.upgrade == "hide") {
                hasHide = true;
                os << "Robot " << id << " upgrades Moving ability: Hide enabled.\n";
            }
        } else if (choice.area == "shooting") {
            if (choice.upgrade == "longShot") {
                hasLongShot = true;
                os << "Robot " << id << " upgrades Shooting ability: Long Shot enabled.\n";
            } else if (choice.upgrade == "semiAuto") {
                hasSemiAuto = true;
                os << "Robot " << id << " upgrades Shooting ability: Semi-Auto enabled.\n";
            } else if (choice.upgrade == "thirtyShot") {
                hasThirtyShot = true;
                os << "Robot " << id << " upgrades Shooting ability: Thirty Shot enabled.\n";
            } else if (choice.upgrade == "laser") {
                hasLaser = true;
                os << "Robot " << id << " upgrades Shooting ability: Laser enabled.\n";
            } else if (choice.upgrade == "sniper") {
                hasSniper = true;
                os << "Robot " << id << " upgrades Shooting ability: Sniper enabled.\n";
            }
        } else if (choice.area == "seeing") {
            if (choice.upgrade == "scout") {
                hasScout = true;
                os << "Robot " << id << " upgrades Seeing ability: Scout enabled.\n";
            } else if (choice.upgrade == "track") {
                hasTrack = true;
                os << "Robot " << id << " upgrades Seeing ability: Track enabled.\n";
            }
        }

        // Mark the area as upgraded (one upgrade per area)
        upgrades.insert(choice.area);
    }

    void actionLaser(vector<Robot*>& robots, int rows, int cols, ostream& os) {
        bool killedAny = false;
        for (int ty = y - 1; ty >= 0; --ty) {
            for (auto it = robots.begin(); it != robots.end(); ++it) {
                if ((*it)->getX() == x && (*it)->getY() == ty) {
                    os << id << " lasers at (" << x << ", " << ty << ") and destroys " << (*it)->getId() << "!\n";
                    killCount++;
                    delete *it;
                    robots.erase(it);
                    killedAny = true;
                    break;
                }
            }
        }
        if (!killedAny)
            os << id << " lasers upwards but kills no robot.\n";
        os << "Number of kills: " << killCount << "\n";
    }

    string getType() const override { 
        return type; 
    }

    string getId() const override { 
        return id; 
    }

    int getX() const override { 
        return x; 
    }

    int getY() const override { 
        return y; 
    }

    int getKillCount() const { 
        return killCount; 
    }

    void think() override {
        // Decide what to do (implement your strategy here)
    }

    void look(const vector<vector<string>>& grid, int rows, int cols, ostream& os, int dx = 0, int dy = 0) const override {
        if (hasScout && scoutCount > 0) {
            os << "Robot " << id << " uses ScoutBot ability to see the whole battlefield!\n";
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    os << (grid[i][j] == "    " ? "EMPTY " : grid[i][j]) << " ";
                }
                os << endl;
            }
        } else {
            int cx = x + dx;
            int cy = y + dy;
            os << "Robot " << id << " (" << type << ") looks at (" << cx << "," << cy << "):\n";
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int nx = cx + i, ny = cy + j;
                    os << "(" << nx << "," << ny << "): ";
                    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) {
                        os << "OUT ";
                    } else if (grid[nx][ny] != "    " && grid[nx][ny] != id) {
                        os << "ENEMY(" << grid[nx][ny] << ") ";
                    } else if (grid[nx][ny] == id) {
                        os << "SELF ";
                    } else {
                        os << "EMPTY ";
                    }
                }
                os << endl;
            }
        }
    }

    void move(int dx, int dy, int rows, int cols) override {
        if (hasJump && jumpCount > 0) {
            x = rand() % rows;
            y = rand() % cols;
            jumpCount--;
        } else {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                x = nx; y = ny;
            }
        }
    }

    bool fire(int dx, int dy, vector<Robot*>& robots, int rows, int cols, ostream& os) override {
        if (shells <= 0) {
            os << "Robot " << id << " is out of shells and self-destructs!\n";
            return true; // Mark for removal in simulation loop
        }
        if (dx == 0 && dy == 0) {
            os << "Robot " << id << " cannot fire at itself!\n";
            return false;
        }
        int tx = x + dx, ty = y + dy;
        if (hasLongShot && (abs(dx) + abs(dy) > 3)) {
            int maxRange = hasLongShot ? 3 : 1;  // Normal range logic
            if (hasSniper) maxRange = 5;  // Sniper increases max range to 5
            int range = abs(dx) + abs(dy);
            if (range > maxRange) {  // Sniper range check
                os << "Robot " << id << " cannot fire that far (max range " << maxRange << ").\n";
                return false;
            }
        }
        if (!hasLongShot && (abs(dx) > 1 || abs(dy) > 1)) {
            os << "Robot " << id << " cannot fire that far (normal range 1).\n";
            return false;
        }
        if (tx < 0 || tx >= rows || ty < 0 || ty >= cols) {
            os << "Robot " << id << " fires out of bounds.\n";
            return false;
        }
        int shots = hasSemiAuto ? 3 : 1;
        shells--;
        if (hasThirtyShot && shells == 0) shells = 30; // reload once
        for (auto it = robots.begin(); it != robots.end(); ++it) {
            if ((*it)->getX() == tx && (*it)->getY() == ty) {
                bool destroyed = false;
                for (int s = 0; s < shots; ++s) {
                    double randProb = static_cast<double>(rand()) / RAND_MAX;
                    if (randProb < 0.7) {
                        destroyed = true;
                        break;
                    }
                }
                if (destroyed) {
                    os << "Robot " << id << " fires at (" << tx << "," << ty << ") and destroys " << (*it)->getId() << "!\n";
                    killCount++;
                    delete *it;
                    robots.erase(it);
                    upgradeRandomChoice(os);  // Call upgrade after a successful kill
                    return false;
                } else {
                    os << "Robot " << id << " fires at (" << tx << "," << ty << ") and misses.\n";
                    return false;
                }
            }
        }
        os << "Robot " << id << " fires at (" << tx << "," << ty << ") but no robot is there.\n";
        return false;
    }

    void upgrade(const string& area, ostream& os) {
        if (area == "moving" && !hasHide && !hasJump) {
            hasJump = true;
            upgrades.insert("moving");
            os << "Robot " << id << " upgrades Moving ability: Jump enabled.\n";
        } else if (area == "shooting" && !hasLongShot && !hasSemiAuto && !hasThirtyShot) {
            hasSemiAuto = true;
            upgrades.insert("shooting");
            os << "Robot " << id << " upgrades Shooting ability: Semi-Auto enabled.\n";
        } else if (area == "seeing" && !hasScout && !hasTrack) {
            hasScout = true;
            upgrades.insert("seeing");
            os << "Robot " << id << " upgrades Seeing ability: Scout enabled.\n";
        } else if (area == "shooting" && !hasLaser) {
            hasLaser = true;
            upgrades.insert("shooting");
            os << "Robot " << id << " upgrades Shooting ability: Laser enabled.\n";
        }
    }
};  // End of GenericRobot class

// For re-entry queue
struct ReEntryInfo {
    string id;
    int lives;
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
    vector<Robot*> robots;
    queue<ReEntryInfo> reentryQueue;

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
        int rx, ry;
        if (xstr == "random" || ystr == "random") {
            rx = rand() % rows;
            ry = rand() % cols;
        } else {
            rx = stoi(xstr);
            ry = stoi(ystr);
        }
        robots.push_back(new GenericRobot(type, id.substr(0, 4), rx, ry, 3));
    }

    // Prepare grid
    vector<vector<string>> grid(rows, vector<string>(cols, "    "));
    auto updateGrid = [&]() {
        grid.assign(rows, vector<string>(cols, "    "));
        for (const auto& r : robots) {
            if (r->getX() >= 0 && r->getX() < rows && r->getY() >= 0 && r->getY() < cols)
                grid[r->getX()][r->getY()] = r->getId();
        }
    };

    auto printGrid = [&](ostream& os) {
        os << "   ";
        for (int j = 0; j < cols; ++j)
            os << setw(5) << j;
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

    updateGrid();
    fout << "Display Battlefield:\n";
    printGrid(fout);

    // Turn-based simulation
    for (int t = 0; t < turns; ++t) {
        // Re-entry: Only one robot per turn
        if (!reentryQueue.empty()) {
            ReEntryInfo info = reentryQueue.front();
            reentryQueue.pop();
            int rx, ry;
            do {
                rx = rand() % rows;
                ry = rand() % cols;
            } while (grid[rx][ry] != "    ");
            robots.push_back(new GenericRobot("GenericRobot", info.id, rx, ry, info.lives));
            fout << "Robot " << info.id << " re-enters the battlefield at (" << rx << "," << ry << ") with " << info.lives << " lives left.\n";
        }

        updateGrid();

        // Each robot acts in order
        for (size_t i = 0; i < robots.size();) {
            Robot* base = robots[i];
            // Look
            if (auto see = dynamic_cast<SeeingRobot*>(base))
                see->look(grid, rows, cols, fout, 0, 0);
            // Move (down-right as example)
            if (auto move = dynamic_cast<MovingRobot*>(base)){
                move->move(1, 1, rows, cols);
            }
               
            // Fire (right as example)
            bool selfDestruct = false;
            if (auto gr = dynamic_cast<GenericRobot*>(base)) {
                if (gr->hasLaser) {
                    gr->actionLaser(robots, rows, cols, fout);
                } else {
                    selfDestruct = gr->fire(0, 1, robots, rows, cols, fout);
                }
            } else if (auto shoot = dynamic_cast<ShootingRobot*>(base)) {
                selfDestruct = shoot->fire(0, 1, robots, rows, cols, fout);
            }

            // Remove robot if self-destructed or destroyed
            if (selfDestruct) {
                if (base->lives > 1) {
                    reentryQueue.push({base->getId(), base->lives - 1});
                }
                delete base;
                robots.erase(robots.begin() + i);
            } else {
                ++i;
            }
            
        }

        fout << "Kill counts after turn " << t + 1 << ":\n";
        for (const auto& r : robots) {
            auto gr = dynamic_cast<GenericRobot*>(r);
            if (gr) {
                fout << gr->getId() << ": " << gr->getKillCount() << " kills\n";
            }
        }
        fout << endl;
        updateGrid();
        fout << "Turn " << t + 1 << ":" << endl;
        printGrid(fout);

        // Stop if only one robot remains
        if (robots.size() <= 1) {
            fout << "Simulation ends: only one robot remains.\n";
            break;
        }
    }

    // Clean up
    for (auto r : robots) delete r;
}

int main() {
    srand(time(NULL)); 

    for (int i = 1; i <= 6; ++i) {
        string inputFile = "fileInput" + to_string(i) + ".txt";
        string outputFile = "fileOutput" + to_string(i) + ".txt";
        processFile(inputFile, outputFile);
    }
    return 0;
}
