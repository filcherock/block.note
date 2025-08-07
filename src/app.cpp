#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>

const std::string FULL_NAME = "Block.Note Text Editor";
const std::string VERSION = "0.0.1";

#define TEXT_GREEN "\033[32m"
#define TEXT_DEFAULT "\033[0m"
#define DBGWT "\033[47;30m"

using namespace std;

string fileNameInput;
string fileTypeInput;

int i = 1;

string line;
ofstream file;

int great() {
    cout << "\t\t Block.Note Text Editor" << endl;
    cout << "\tSmall CLI text editor written in C++" << endl;
    cout << "\tfor quit, please enter ':q'. Good Luck <3" << endl;
    return 0;
}

int editor() {
    cout << TEXT_GREEN;
    cout.width(3);
    cout << i++ << "  " << TEXT_DEFAULT;
    getline(cin, line);
    while (getline(cin, line))
    {
        if (line == ":q") break;
        file << line << endl;
        cout << TEXT_GREEN;
        cout.width(3);
        cout << i++ << "  " << TEXT_DEFAULT;
    }
    return 0;
}


int openFile(string fileName, string fileType) {
    file.open(fileName + "." + fileType);
    system("clear");
    cout << DBGWT;
    cout << FULL_NAME << " v." << VERSION;
    cout << "  Editing: " << fileName + "." + fileType << TEXT_DEFAULT << endl << endl;
    editor();
}

int main() {
    system("clear");
    great();
    cout << "Please, enter the file name: ";
    cin >> fileNameInput;
    cout << "Please, enter file type (txt, bnt, doc ...): ";
    cin >> fileTypeInput;
    openFile(fileNameInput, fileTypeInput);
    return 0;
}