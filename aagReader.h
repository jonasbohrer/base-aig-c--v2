#ifndef AAGREADER_H
#define AAGREADER_H
#include "aig.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

class AAGReader
{
private:
    ifstream source;
    ofstream debug;
    string aigname;
    string word;
    char buf[250];

public:
    AAGReader(string sourcePath);
    Aig* readFile();
    AigNode* findNode(string name, AigNode** nodes, int aigSize);
    bool isInverted(int value);
};

#endif // AAGREADER_H
