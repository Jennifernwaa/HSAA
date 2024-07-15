#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdlib> // for getpass debugging, exit(EXIT_SUCCESS); // or exit(EXIT_FAILURE) for an error exit status

using namespace std;

// clear screen related
#include <stdlib.h> // for system() function to execute a system command
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

// password related
#include <cctype>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// menu related
#ifndef _MENU_OPTIONS
vector<pair<string, string> > menu_options;
#define _MENU_OPTIONS
#endif

// input choices related
#ifndef _INPUT_CHOICES
vector<pair<string, string> > input_choices;
#define _INPUT_CHOICES
#endif

// input related
char getHiddenChar() {
    #ifdef _WIN32
        return _getch();
    #else
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    #endif
}

// ------------------------------------------------------------------------------------
class UI {
public:

    // clear screem
    static void clearScreen() {
        system(CLEAR); // "cls" For Windows, "clear" For Linux and Unix       
    }

    // clear input buffer
    // static void clearInputBuffer() {
    //     // Clear the input buffer to allow password input
    //     cin.clear();
    //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // }

    static void clearInputBuffer() {
        char c;
        while ((c = cin.get()) != '\n' && c != EOF);
    }

    // showPressAnyKey
    static void  showPressAnyKey(const string& label = "Press any key to continue...") {
        cout << label;
        cout.flush();
        
        getHiddenChar(); // Wait for a key press

        cout << endl;
    }

    // .......................................................................
    // input related

    // showInputText (must not contain a space, if it is then only the chars before the spce will be returned)
    static string showInputText(const string& label, int padding) {
        cout << strPadEnd(label, padding) << ": ";
        string input;
        cin >> input;
        return input;
    }

    // showInputTextLine (can contain spaces)
    // it may need clearInputBuffer(); before, depending in the case if after another cin
    static string showInputTextLine(const string& label, int padding) {
        cout << strPadEnd(label, padding) << ": ";
        string input;

        // Peek at the next character
        int nextChar = cin.peek();
        // Check if it's a newline character
        if (nextChar == '\n') {
            // Ignore the newline character
            cin.ignore();
        }

        // read user input
        getline(cin, input);
        return input;
    }

    // showInputPassword
    static string showInputPassword(const string& label, int padding) {
        cout << strPadEnd(label, padding) << ": ";
        
        cout.flush(); // Ensure the prompt is displayed before input

        string password;
        char c;

        clearInputBuffer();

        while (true) {
            c = getHiddenChar();

            if (c == '\n' || c == '\r') {
                // Enter key was pressed, end input
                cout << endl;
                return password;
            }

            if (isprint(c)) {
                // Display an asterisk for printable characters
                cout << '*';
                password += c;
            }
        }
    }

    // showInputInt
    static int showInputInt(const string& label, int padding) {
        while (true) {
            cout << strPadEnd(label, padding) << ": ";
            string input;
            cin >> input;

            if (input.empty()) {
                // If the input is empty, prompt the user again.
                continue;
            }

            // Use stringstream to parse the input as an integer
            istringstream ss(input);
            int intValue;

            if (ss >> intValue) {
                // Successfully parsed as an integer
                return intValue;
            } else {
                cout << "Invalid input. Please enter a valid integer." << endl;
            }
        }
    }

    // showChoices
    static void initChoices() {
        input_choices.clear();
    }

    static void addChoice(const string& key, const string& label) {
         input_choices.push_back(make_pair(key, label));
    }

    static string showChoices(const string& label, int padding) {
        while (true) {
            cout << strPadEnd(label, padding) << ": "; 

            string indent = "";

            for (size_t i = 0; i < input_choices.size(); ++i) {
                if (i > 0) {
                    indent = string(padding + 2, ' ');
                }
                cout << indent << input_choices[i].first << ": " << input_choices[i].second << endl;
            }

            string prompt = "Enter your choice";
            cout << strPadEnd(prompt, padding) << ": ";
            string input;
            cin >> input;

            for (size_t i = 0; i < input_choices.size(); ++i) {
                if (input == input_choices[i].first) {
                    return input_choices[i].first;
                }
            }

            cout << strPadEnd("", padding+2) << "Invalid choice. Try again." << endl;
        }
    }

    // .......................................................................
    // menu related
    static void initMenuOptions () {
        menu_options.clear();
    }

    static void addMenuOption ( const string& key, const string& label) {
        menu_options.push_back(make_pair(key, label));
    }

    static string getMenuLabel(const string& key) {
        for (vector<pair<string, string> >::const_iterator it = menu_options.begin(); it != menu_options.end(); ++it) {
            if (it->first == key) {
                return it->second; // Return the label when a matching key is found
            }
        }
        // Return an empty string or some default value if the key is not found
        return "";
    }

    static vector<pair<string, string> > getMenuOptions() {
        return menu_options;
    }

    // showmenu_options, pass the result of getMenuOptions
    static string showMenuOptions(const string& menuTitle, const string& promptLabel = "Select a menu option", const string& chosenLabel = "Option selected") {
        cout << menuTitle << endl;
        cout << endl;

        for (size_t i = 0; i < menu_options.size(); ++i) {
            cout << menu_options[i].first << " - " << menu_options[i].second << endl;
        }

        cout << endl;
        cout << promptLabel << ": ";

        string input;
        while (true) {
            cin >> input;
            transform(input.begin(), input.end(), input.begin(), ::tolower);
            if (isValidOption(menu_options, input)) {
                cout << chosenLabel << ": " << findLabelById(menu_options, input) << endl;
                return input;
            } else {
                cout << "Invalid option. Try again: ";
            }
        }
    }

    // .......................................................................
    // table related
    static void tableSeparatorRow(int numCols, const int colWidth[]) {
        cout << "+";
        for (int i = 0; i < numCols; i++) {
            for (int j = 0; j < colWidth[i] + 2; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
    }

    static void showTable(const string& title, const string headers[], const vector<vector<string> >& rows, const int col_sizes[], const int num_cols) {
        int numCols = num_cols + 1; // plus the first "#"" col
  
        // Calculate the total table width including margins
        int totalWidth = 1; // Initialize with 1 for the left border
        int colWidth[numCols];
        colWidth[0] = 2; // Width of the "#" column
        totalWidth += colWidth[0];

        for (int i = 1; i < numCols; i++) {
            colWidth[i] = col_sizes[i - 1];
            totalWidth += colWidth[i] + 4; // Col size + 4 for left and right margins
        }

        // Calculate the number of rows based on the size of the rows vector
        int num_rows = rows.size();

        // Display the title without borders
        cout << title << endl;

        // Display the table top border
        tableSeparatorRow(numCols, colWidth);
        
        // Display the header row
        cout << "|  #";
        for (int i = 1; i < numCols; i++) {
            string header = headers[i - 1];
            cout << " | " << setw(colWidth[i]) << left << header;
        }
        cout << " |" << endl;

        // Display the separator row
        tableSeparatorRow(numCols, colWidth);
        
        // Display each row with row numbers
        for (int row = 0; row < num_rows; row++) {
            cout << "| " << setw(2) << right << row + 1;
            for (int i = 1; i < numCols; i++) {
                string cell = rows[row][i - 1];
                cout << " | " << setw(colWidth[i]) << left << cell;
            }
            cout << " |" << endl;
        }

        // Display the table bottom border
        tableSeparatorRow(numCols, colWidth);
    }

    // .......................................................................
    // misc methods
    static string getCurrentDate() {
        // Get the current time
        time_t now;
        time(&now);
        struct tm* timeinfo = localtime(&now);

        // // Format the date as DD-MM-YYYY
        // char buffer[11]; // To store "DD-MM-YYYY\0"
        // strftime(buffer, 11, "%d-%m-%Y", timeinfo);

        // Format the date as DD-Mon-YYYY (e.g., 01-Nov-2023)
        char buffer[12]; // To store "DD-Mon-YYYY\0"
        strftime(buffer, 12, "%d %b %Y", timeinfo);

        return string(buffer);
    }

    // showEmptyLine
    static void showEmptyLine(int lines = 1) {
        for (int i = 0; i < lines; ++i) {
            cout << endl;
        }
    }

    // showRepeatChar
    static void showRepeatChar(char character = '-', int num_repeat = 80) {
        for (int i = 0; i < num_repeat; ++i) {
            cout << character;
        }
        cout << endl;
    }

    // string display
    static void showLine (const string& line) {
        cout << line << endl;
    }

    // showLines (display array of strings)
    template <size_t N>
    static void showLines(const string (&lines)[N]) {
        for (size_t i = 0; i < N; i++) {
            cout << lines[i] << endl;
        }
    }

    // string trim
    static string strTrim(const string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        
        if (start == string::npos || end == string::npos)
            return "";

        return str.substr(start, end - start + 1);
    }

    // string padding
    static string strPadEnd (const string& label, int padding) {
        string trimmed = strTrim(label);
        if (padding <= trimmed.size()) {
            return trimmed;
        } 
        string spaces(padding - trimmed.size(), ' ');
        return trimmed + spaces;
    }
    
    static string strPadStart (const string& label, int padding) {
        string trimmed = strTrim(label);
        if (padding <= trimmed.size()) {
            return trimmed;
        } 
        string spaces(padding - trimmed.size(), ' ');
        return spaces + trimmed;
    }

    // string replace
    static string strReplace(const string& str, const string& find, const string& replace = "") {
        // If the replacement string is empty, simply remove occurrences of the 'find' string
        if (replace.empty()) {
            size_t pos = 0;
            string result = str;

            while ((pos = result.find(find, pos)) != string::npos) {
                result.erase(pos, find.length());
            }

            return result;
        }

        // Otherwise, perform the standard replacement logic
        size_t pos = 0;
        string result = str;

        while ((pos = result.find(find, pos)) != string::npos) {
            result.replace(pos, find.length(), replace);
            pos += replace.length();
        }

        return result;

    }


private:

    static string findLabelById(const vector<pair<string, string> >& options, const string& id) {
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i].first == id) {
                return options[i].second;
            }
        }
        return "";
    }

    static bool isValidOption(const vector<pair<string, string> >& options, const string& id) {
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i].first == id) {
                return true;
            }
        }
        return false;
    }
};
