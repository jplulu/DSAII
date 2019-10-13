#include "hashTable.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <cstring>

using namespace std;

void loadDict(hashTable* myhash, const string& dict);
void spellcheck(hashTable* myhash, const string& inFile, const string& outFile);

int main() {
    hashTable* myhash = new hashTable(50000);
    string dict, inFile, outFile;
    cout << "Enter name of dictionary: " << flush;
    cin >> dict;
    clock_t t1 = clock();
    loadDict(myhash, dict);
    clock_t t2 = clock();
    cout << "Total time (in seconds) to load dictionary: " << double(t2 - t1) / CLOCKS_PER_SEC << endl;

    cout << "Enter name of input file: " << flush;
    cin >> inFile;
    cout << "Enter name of output file: " << flush;
    cin >> outFile;
    t1 = clock();
    spellcheck(myhash, inFile, outFile);
    t2 = clock();
    cout << "Total time (in seconds) to check document: " << double(t2 - t1) / CLOCKS_PER_SEC << endl;

    return 0;
}

void loadDict(hashTable* myhash, const string& dict)
{
    ifstream Dictionary;
    Dictionary.open(dict);
    if(Dictionary.is_open())
    {
        string line;
        while(getline(Dictionary, line))
        {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            myhash->insert(line);
        }
    } else {
        cerr << "Error: Failed to open file for reading: " << dict << " " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    Dictionary.close();
}

void spellcheck(hashTable* myhash, const string& inFile, const string& outFile)
{
    ifstream inputFile;
    inputFile.open(inFile);
    if(!inputFile.is_open())
    {
        cerr << "Error: Failed to open file for reading: " << inFile << " " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    ofstream outputFile;
    outputFile.open(outFile);
    if(!outputFile.is_open())
    {
        cerr << "Error: Failed to open file for writing: " << outFile << " " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    string line = "", validWord = "";
    char tempChar;
    unsigned long lineNum = 0;
    bool containsNum = false;
    while(getline(inputFile, line))
    {
        int i = 0;
        lineNum++;
        while(i <= line.size())
        {
            tempChar = tolower(line[i++]);
            if(isdigit(tempChar)) containsNum = true;
            if((isalnum(tempChar))||(tempChar == '-')||(tempChar == '\''))
                validWord += tempChar;
            else if(!validWord.empty() && !containsNum)
            {
                if(validWord.size() > 20)
                    outputFile << "Long word at line " << lineNum << ", starts: " << validWord.substr(0, 20) << endl;
                else if(!myhash->contains(validWord))
                    outputFile << "Unknown word at line " << lineNum << ": " << validWord << endl;
                validWord = "";
            } else {
                validWord = "";
                containsNum = false;
            }
        }
    }
    inputFile.close();
    outputFile.close();

}

