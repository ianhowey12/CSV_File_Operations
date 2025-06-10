#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

string csvReadPath = "";
string csvWritePath = "";

string csvTitle = "";

string csvData = "";

int csvNumLines = 0;
int csvNumChars = 0;
vector<string> csvLines;

bool csvDataUpdated = 1;

#include "fileops.hpp"

// get number of lines in stored data
int getNumStoredLines(){
    return csvNumLines;
}

// get number of chars in stored data, including newline characters following each line
int getNumStoredChars(){
    return csvNumChars;
}

// get the stored data
string getStoredData(){
    if(!csvDataUpdated){
        csvData = "";
        for(int i=0;i<csvNumLines;i++){
            csvData += csvLines[i] + "\n";
        }
    }
    return csvData;
}

// get a line (0-indexed) of the stored data
string getStoredLine(int lineIndex){
    if(lineIndex < -1 * csvNumLines){
        cerr << "Tried to get stored data line " << lineIndex << " (" << csvNumLines + lineIndex << " from start) when only " << csvNumLines << " lines were present.\n\n";
        exit(1);
    }
    if(lineIndex < 0){
        lineIndex += csvNumLines;
    }
    if(lineIndex >= csvNumLines){
        cerr << "Tried to get stored data line " << lineIndex << " when only " << csvNumLines << " lines were present.\n\n";
        exit(1);
    }
    
    csvDataUpdated = 1;
    return csvLines[lineIndex];
}

// set the stored data
void setStoredData(string data){
    
    csvData = data;

    int n = size(data);
    csvNumChars = n;
    csvLines.clear();

    // find each line
    string line = "";
    for(int i=0;i<n;i++){
        if(data[i] == '\n'){
            csvNumLines++;
            csvLines.push_back(line);
            line = "";
        }else{
            line += data[i];
        }
    }
    csvNumLines++;
    csvLines.push_back(line);
    csvDataUpdated = 1;
}

// set a line (0-indexed) of the stored data
void setStoredDataLine(string lineData, int lineIndex){
    if(lineIndex < -1 * csvNumLines){
        cerr << "Tried to set stored data line " << lineIndex << " (" << csvNumLines + lineIndex << " from start) when only " << csvNumLines << " lines were present.\n\n";
        exit(1);
    }
    if(lineIndex < 0){
        lineIndex += csvNumLines;
    }
    if(lineIndex >= csvNumLines){
        cerr << "Tried to set stored data line " << lineIndex << " when only " << csvNumLines << " lines were present.\n\n";
        exit(1);
    }

    csvNumChars += size(lineData) - size(csvLines[lineIndex]);
    csvLines[lineIndex] = lineData;
    csvDataUpdated = 0;
}

// read stored data from a csv file of data lines, adding onto current stored data if additive
void readFileToData(string path, bool additive) {
    ifstream f(path);
    string line = "";

    if(!additive){
        csvNumChars = 0;
        csvNumLines = 0;
        csvLines.clear();
        csvData = "";
    }

    while(getline(f, line)) {
        csvLines.push_back(line);
        csvData += line + '\n';
        csvNumChars += size(line) + 1;
        csvNumLines++;
    }

    f.close();
    csvDataUpdated = 1;
}

// read stored data from a csv file of data lines
string readFileToString(string path) {
    ifstream f(path);
    string line = "";
    string o = "";

    while(getline(f, line)) {
        o += line + '\n';
    }

    f.close();
    return o;
}

// read stored data from a csv file of data lines
vector<string> readFileToVector(string path) {
    ifstream f(path);
    string line = "";
    vector<string> o;

    while(getline(f, line)) {
        o.push_back(line);
    }

    f.close();
    return o;
}

// write stored data to a csv file of data lines, adding onto current file data if additive
void writeDataToFile(string path, bool additive) {

    if(additive){
        vector<string> lines = readFileToVector(path);

        // add stored data onto csv file data
        for(int i=0;i<csvNumLines;i++){
            lines.push_back(csvLines[i]);
        }

        csvLines = lines;
        csvNumLines = size(lines);

        csvNumChars = 0;
        csvData = "";
        for(int i=0;i<csvNumLines;i++){
            csvNumChars += size(csvLines[i]) + 1;
            csvData += csvLines[i] + "\n";
        }
    }else{

        if(!csvDataUpdated){
            csvNumChars = 0;
            csvData = "";
            for(int i=0;i<csvNumLines;i++){
                csvNumChars += size(csvLines[i]) + 1;
                csvData += csvLines[i] + "\n";
            }
        }

    }
    ofstream f(path);
    f << csvData;
    f.close();
    csvDataUpdated = 1;
}

// read a folder of folders, write their titles as data lines to destPath
void combineFolderTitlesToLines(string sourcePath, string destPath, bool includeExtensions, bool includeFullPaths) {

    csvData = "";
    csvNumLines = 0;
    csvNumChars = 0;
    csvLines.clear();
    
    fs::path source{sourcePath};
    for(const auto& entry : fs::directory_iterator(source)){

        string line = entry.path().string();
        if(!includeFullPaths){
            line = line.substr(size(sourcePath) + 1);
        }
        if(!includeExtensions){
            for(int i=size(line);i>=0;i--){
                if(line[i] == '.'){
                    line = line.substr(0, i); break;
                }
            }
        }
        csvLines.push_back(line);
        line.append("\n");
        csvData += line;
        csvNumChars += size(line) + 1;
        csvNumLines++;
    }

    writeDataToFile(destPath, 1);
    csvDataUpdated = 1;
}

// combine file data, store the combined data in globals
void combineFiles(vector<string> filePaths){
    int n = size(filePaths);

    csvNumChars = 0;
    csvLines.clear();
    csvNumLines = 0;
    csvData = "";

    for(int i=0;i<n;i++){
        readFileToData(filePaths[i], 1);
    }
    
    csvDataUpdated = 1;
}
