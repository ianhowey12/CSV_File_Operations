#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int getNumStoredLines();

int getNumStoredChars();

string getStoredData();

string getStoredLine(int lineIndex);

string getStoredData();

string getStoredLine(int lineIndex);

void setStoredData(string data);

void setStoredDataLine(string lineData, int lineIndex);

void readFileToData(string path, bool additive);

string readFileToString(string path);

vector<string> readFileToVector(string path);

void writeDataToFile(string path, bool additive);

void combineFolderTitlesToLines(string sourcePath, string destPath, bool includeExtensions, bool includeFullPaths);

void combineFiles(vector<string> filePaths);
