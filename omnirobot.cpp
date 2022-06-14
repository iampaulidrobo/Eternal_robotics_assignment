#include <string>
#include <fstream>
#include <vector>
#include <utility>   // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream>   // std::stringstream
#include <iostream>
#include <cmath>

using namespace std;
vector<double> S1;
vector<double> S2;
vector<double> Angletheta;
vector<double> direction;
vector<double> distanceToAchieve;
vector<double> M1;
vector<double> M2;
vector<double> M3;
vector<double> M4;
vector<double> M1d;
vector<double> M2d;
vector<double> M3d;
vector<double> M4d;
double arrivingDistance = 30;
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<double>>> dataset)
{
    std::ofstream myFile(filename);
    for (int j = 0; j < dataset.size(); ++j)
    {
        myFile << dataset.at(j).first;
        if (j != dataset.size() - 1)
            myFile << ",";
    }
    myFile << "\n";
    for (int i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for (int j = 0; j < dataset.size(); ++j)
        {
            if (dataset.at(j).first == "Direction")
            {
                if (dataset.at(j).second.at(i) == 1)
                {
                    // myFile << dataset.at(j).second.at(i);
                    myFile << "CLOCKWISE";
                }
                else if (dataset.at(j).second.at(i) == -1)
                {
                    myFile << "COUNTER-CLOCKWISE";
                }
                else if (dataset.at(j).second.at(i) == 0)
                {
                    myFile << "NULL";
                }
            }
            else
            {
                myFile << dataset.at(j).second.at(i);
            }

            if (j != dataset.size() - 1)
                myFile << ",";
        }
        myFile << "\n";
    }
    myFile.close();
}
void read_csv(std::string filename)
{
    std::vector<std::pair<std::string, std::vector<int>>> result;
    std::ifstream myFile(filename);
    if (!myFile.is_open())
        throw std::runtime_error("Could not open file");

    std::string line, colname;
    double val;

    // Read the column names
    if (myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while (std::getline(ss, colname, ','))
        {

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<int>{}});
        }
    }
    // Read data, line by line
    while (std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;
        // cout<<"c"<<endl;

        // Extract each integer
        while (ss >> val)
        {

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if (ss.peek() == ',')
                ss.ignore();

            // Increment the column index
            if (colIdx == 0)
            {
                S1.push_back(val);
            }
            else if (colIdx == 1)
            {
                S2.push_back(val);
            }
            colIdx++;
        }
    }

    vector<double>::iterator iS1;
    vector<double>::iterator iS2;
    iS2 = S2.begin();
    for (iS1 = S1.begin(); iS1 != S1.end(); ++iS1)
    {
        // Direction
        if (*iS1 - *iS2 > 0)
        {
            direction.push_back(1);
        }
        else if (*iS1 - *iS2 < 0)
        {
            direction.push_back(-1);
        }
        else
        {
            direction.push_back(0);
        }
        // Angletheta in radians
        Angletheta.push_back(((atan((*iS1 - *iS2) / 30))) * 180 / 3.1415);
        distanceToAchieve.push_back(arrivingDistance - min(*iS1, *iS2));
        ++iS2;
    }

    // distance travelled in 1 pulse
    double onepulsedistance = ((2 * 3.14 * 30) / 2000 / 30);
    vector<double>::iterator idistanceToAchieve;
    idistanceToAchieve = distanceToAchieve.begin();
    for (auto &s : Angletheta)
    {
        auto ts = s / 2;

        if (ts > 0)
        {
            M1.push_back(tan(ts * 3.14 / 180) / onepulsedistance);
            M2.push_back(tan(ts * 3.14 / 180) / onepulsedistance);
            M3.push_back(tan(ts * 3.14 / 180) / onepulsedistance);
            M4.push_back(tan(ts * 3.14 / 180) / onepulsedistance);
        }
        else if (ts < 0)
        {
            ts = -ts;
            M1.push_back(-tan(ts * 3.14 / 180) / onepulsedistance);
            M2.push_back(-tan(ts * 3.14 / 180) / onepulsedistance);
            M3.push_back(-tan(ts * 3.14 / 180) / onepulsedistance);
            M4.push_back(-tan(ts * 3.14 / 180) / onepulsedistance);
        }
        else
        {
            M1.push_back(0);
            M2.push_back(0);
            M3.push_back(0);
            M4.push_back(0);
        }
        // pulses required to move 30cm parallel to wall
        M1d.push_back(*idistanceToAchieve / onepulsedistance);
        M2d.push_back(*idistanceToAchieve / onepulsedistance);
        M3d.push_back(*idistanceToAchieve / onepulsedistance);
        M4d.push_back(*idistanceToAchieve / onepulsedistance);
        ++idistanceToAchieve;
    }

    // Close file
    myFile.close();
}

int main()
{

    read_csv("input.csv");
    std::vector<std::pair<std::string, std::vector<double>>> vals = {{"Angletheta", Angletheta},
                                                                     {"M1", M1},
                                                                     {"M2", M2},
                                                                     {"M3", M3},
                                                                     {"M4", M4},
                                                                     {"Direction", direction},
                                                                     {"M1", M1d},
                                                                     {"M2", M2d},
                                                                     {"M3", M3d},
                                                                     {"M4", M4d},
                                                                     {"Direction", direction}};
    write_csv("output.csv", vals);

    return 0;
}