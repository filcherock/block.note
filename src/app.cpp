/*
 /$$$$$$$  /$$        /$$$$$$   /$$$$$$  /$$   /$$     /$$   /$$  /$$$$$$  /$$$$$$$$ /$$$$$$$$
| $$__  $$| $$       /$$__  $$ /$$__  $$| $$  /$$/    | $$$ | $$ /$$__  $$|__  $$__/| $$_____/
| $$  \ $$| $$      | $$  \ $$| $$  \__/| $$ /$$/     | $$$$| $$| $$  \ $$   | $$   | $$      
| $$$$$$$ | $$      | $$  | $$| $$      | $$$$$/      | $$ $$ $$| $$  | $$   | $$   | $$$$$   
| $$__  $$| $$      | $$  | $$| $$      | $$  $$      | $$  $$$$| $$  | $$   | $$   | $$__/   
| $$  \ $$| $$      | $$  | $$| $$    $$| $$\  $$     | $$\  $$$| $$  | $$   | $$   | $$      
| $$$$$$$/| $$$$$$$$|  $$$$$$/|  $$$$$$/| $$ \  $$ /$$| $$ \  $$|  $$$$$$/   | $$   | $$$$$$$$
|_______/ |________/ \______/  \______/ |__/  \__/|__/|__/  \__/ \______/    |__/   |________/
==============================================================================================
==============================================================================================
Block.Note Text Editor v0.1.0 by filcher
Author: filcher (https://github.com/filcherock)
Official repos: https://github.com/filcherock/block.note
License: GNU General Public License v3.0 (https://github.com/filcherock/block.note/blob/main/LICENSE)
Version: 0.1.0
*/

// Include library
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ncurses.h>

// Create constant
const std::string FULL_NAME = "Block.Note Text Editor";
const std::string VERSION = "0.1.0";

// Using namespace
using namespace std;

// Create vars
string fileNameInput;
string fileTypeInput;
vector<string> lines;

// Create cursor position
int cursorX = 0; 
int cursorY = 0;
int offsetY = 0;

// Split text function
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Editor display
void display() {
    clear();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(1));
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    std::string header = FULL_NAME + " v" + VERSION;
    std::string editing = "Editing: " + fileNameInput + "." + fileTypeInput;

    printw("%-*s", maxX - editing.length() - 1, header.c_str());
    printw("%s\n", editing.c_str());

    attroff(COLOR_PAIR(1));

    int screenLines = maxY - 2;
    if (offsetY > (int)lines.size() - screenLines) {
        offsetY = std::max(0, (int)lines.size() - screenLines);
    }

    for (int i = 0; i < screenLines; ++i) {
        int lineIdx = offsetY + i;
        if (lineIdx >= (int)lines.size()) break;
        printw("%s\n", lines[lineIdx].c_str());
    }

    int cursorScreenY = cursorY - offsetY + 1; 
    if (cursorScreenY < 1) cursorScreenY = 1;
    if (cursorScreenY >= maxY) cursorScreenY = maxY - 1;
    if (cursorX > (int)lines[cursorY].size()) cursorX = (int)lines[cursorY].size();
    if (cursorX < 0) cursorX = 0;

    move(cursorScreenY, cursorX);
    refresh();
}

// Update offset
void updateOffset(int maxY) {
    int screenLines = maxY - 3;
    if (cursorY < offsetY) {
        offsetY = cursorY;
    }
    else if (cursorY >= offsetY + screenLines) {
        offsetY = cursorY - screenLines + 1;
    }
}

// Save file
void saveToFile() {
    ofstream ofs(fileNameInput + "." + fileTypeInput);
    if (!ofs) {
        return;
    }
    for (auto &line : lines) {
        ofs << line << '\n';
    }
    ofs.close();
}

// Editor control
void editor() {
    while (true) {
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        updateOffset(maxY);

        display();
        int ch = getch();

        if (ch == KEY_UP) {
            if (cursorY > 0) {
                cursorY--;
                if (cursorX > (int)lines[cursorY].size()) {
                    cursorX = (int)lines[cursorY].size();
                }
            }
        } else if (ch == KEY_DOWN) {
            if (cursorY < (int)lines.size() - 1) {
                cursorY++;
                if (cursorX > (int)lines[cursorY].size()) {
                    cursorX = (int)lines[cursorY].size();
                }
            }
        } else if (ch == KEY_LEFT) {
            if (cursorX > 0) {
                cursorX--;
            } else if (cursorY > 0) {
                cursorY--;
                cursorX = (int)lines[cursorY].size();
            }
        } else if (ch == KEY_RIGHT) {
            if (cursorX < (int)lines[cursorY].size()) {
                cursorX++;
            } else if (cursorY < (int)lines.size() -1 ) {
                cursorY++;
                cursorX = 0;
            }
        } else if (ch == '\n' || ch == KEY_ENTER) {
            string newLine = lines[cursorY].substr(cursorX);
            lines[cursorY] = lines[cursorY].substr(0, cursorX);
            lines.insert(lines.begin() + cursorY + 1, newLine);
            cursorY++;
            cursorX = 0;
        } else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (cursorX > 0) {
                lines[cursorY].erase(cursorX - 1, 1);
                cursorX--;
            } else if (cursorY > 0) {
                int prevLineLength = (int)lines[cursorY - 1].size();

                lines[cursorY - 1] += lines[cursorY];
                lines.erase(lines.begin() + cursorY);
                cursorY--;
                cursorX = prevLineLength;
            }
        } else if (ch == ':') {
            printw("Command (q/wq): ");
            echo();
            char command[10];
            getnstr(command, 9);
            noecho();
            if (string(command) == "q") {
                break;
            } else if (string(command) == "wq") {
                saveToFile();
                break;
            } else if (string(command) == "w") {
                saveToFile();
            } else {
                printw("Unknown command!\n");
                getch();
            }
        } else if (isprint(ch)) {
            lines[cursorY].insert(cursorX, 1, ch);
            cursorX++;
        }
    }
}

// Open File
void openFile(string fileName, string fileType) {
    ifstream file(fileName + "." + fileType);

    fileNameInput = fileName;
    fileTypeInput = fileType;
    lines.clear();

    if (file) {
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }

    if (lines.empty()) {
        lines.push_back("");
    }
    cursorX = 0;
    cursorY = 0;

    editor();
}

// Great function
int great() {
    system("clear");
    cout << "\t\t Block.Note Text Editor" << endl;
    cout << "\tSmall CLI text editor written in C++" << endl;
    cout << "\tfor quit, please enter ':q'. Good Luck <3" << endl;
    return 0;
}

// Main function
int main(int argc, char* argv[]) {
    initscr();
    cbreak();
    noecho();
    start_color(); 
    keypad(stdscr, TRUE);

    if (argc == 1) {
        endwin();
        great();
        cout << "Please, enter the file name (without extension): ";
        cin >> fileNameInput;
        cout << "Please, enter file type (txt, bnt, doc ...): ";
        cin >> fileTypeInput;
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        openFile(fileNameInput, fileTypeInput);

    } else if (argc == 2) {
        string arg = argv[1];
        size_t dotPos = arg.find('.');
        if (dotPos == string::npos) {
            endwin();
            cout << "File extension missing in argument.\n";
            return 1;
        } 
        fileNameInput = arg.substr(0, dotPos);
        fileTypeInput = arg.substr(dotPos + 1);

        openFile(fileNameInput, fileTypeInput);

    } else {
        endwin();
        cout << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }

    endwin();
    return 0;
}