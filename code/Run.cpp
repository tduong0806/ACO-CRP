#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

int main() {
    string exe_name = "Aco-Duong.exe";
    string input_dir = "..\\input\\gulic\\6-6-31";
    string output_dir = "..\\output\\gulic\\6-6-31";
    string output_file = output_dir + "\\all_results.txt";
    string temp_list = "files_list.txt";

    system(("powershell -Command \"if (!(Test-Path '" + output_dir + "')) { New-Item -ItemType Directory -Force -Path '" + output_dir + "' }\"").c_str());
    system(("dir /b \"" + input_dir + "\" > " + temp_list).c_str());

    ifstream fileList(temp_list);
    ofstream finalOut(output_file);

    string filename;
    double sum = 0;
    int count = 0;

    while (fileList >> filename) {
        if (filename == "all_results.txt" || filename == "files_list.txt") continue;

        cout << "Current Test: " << filename << " ... ";

        string input_path = input_dir + "\\" + filename;
        string temp_out = "temp_single.txt";

        string command = exe_name + " < \"" + input_path + "\" > " + temp_out;
        system(command.c_str());

        ifstream singleRes(temp_out);
        long long value;
        if (singleRes >> value) {
            finalOut << filename << ": " << value << endl;
            sum += value;
            count++;
            cout << "Done! (Value: " << value << ")" << endl;
        } else {
            cout << "Failed to read output!" << endl;
        }
        singleRes.close();
    }

    if (count > 0) {
        double average = sum / count;
        finalOut << "-----------------------------" << endl;
        finalOut << "Average: " << fixed << setprecision(2) << average << endl;

        cout << "=============================" << endl;
        cout << "All tests finished!" << endl;
        cout << "Total files: " << count << endl;
        cout << "Average: " << fixed << setprecision(2) << average << endl;
    }

    fileList.close();
    finalOut.close();

    system(("del " + temp_list).c_str());
    system("del temp_single.txt");

    return 0;
}
