//
//  wgrep.cpp
//  
//
//  Created by Kyle on 4/8/21.
//

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

// searchKey() Looks for Searchterm in Buffered Line and Writes the Line if Found
void searchKey(string &text, char *pattern) {
    int textSize = text.size();
    // Skip if Line Empty
    if (textSize <= 0)
        return;
    int len = strlen(pattern);
    // Search Buffered Line from Each Character
    // Skip Characters if Searchterm Doesn't Fit in Remaining Line
    for (int j = 0; j <= textSize - len; j++) {
        int match = 0; // Number of Matched Characters to Searchterm Starting from Index j
        int k = 0; // Offset
        while (text[j + k] == pattern[k]) {
            match++;
            if (k == len)
                break; // Don't Pass Searchterm Length
            k++;
        }
        if (match == len) {
            // Convert to C-String to Use write()
            char buf[textSize + 1];
            strcpy(buf, text.c_str());
            // Write Line if Searchterm Found
            write(STDOUT_FILENO, buf, textSize);
            return;
        }
    }
}

// wgrep() Writes out Lines of Input File(s) that Contain Given Searchterm
int main(int argc, char *argv[]) {
    // Exit if No Given Searchterm and/or Files
    if (argc == 1) {
        cout << "wgrep: searchterm [file ...]" << endl;
        exit(1);
    }
    char tempChar;
    string buffer;
    size_t bytesRead;
    // Only Searchterm, Read STDIN
    if (argc == 2) {
        // Attempt to Read at Least Once, Don't Repeat if No Bytes Read
        do {
            bytesRead = read(STDIN_FILENO, &tempChar, 1);
            if (bytesRead > 0) {
                buffer.push_back(tempChar);     // add char to end of buffer
                if (tempChar == '\n') {         // reached end of line
                    searchKey(buffer, argv[1]); // search for searchterm in buffer line
                    buffer.clear();             // clear buffer for next line
                }
            }
        }
        while (bytesRead > 0);
        searchKey(buffer, argv[1]);             // search last line of file (if no end of line char)
        buffer.clear();                         // clear buffer
        return(0);
    }
    // Searchterm and Input Files
    int fileDescriptor;
    // For Input Files (3rd argument and up)
    for (int i = 2; i < argc; i++) {
        fileDescriptor = open(argv[i], O_RDONLY);
        // Check if Open Successful
        if (fileDescriptor < 0) {
            cout << "wgrep: cannot open file" << endl;
            exit(1);
        }
        // Attempt to Read at Least Once, Don't Repeat if No Bytes Read
        do {
            bytesRead = read(fileDescriptor, &tempChar, 1);
            if (bytesRead > 0) {
                buffer.push_back(tempChar);     // add char to end of buffer
                if (tempChar == '\n') {         // reached end of line
                    searchKey(buffer, argv[1]); // search for searchterm in buffer line
                    buffer.clear();             // clear buffer for next line
                }
            }
        }
        while (bytesRead > 0);
        searchKey(buffer, argv[1]);             // search last line of file (if no end of line char)
        buffer.clear();                         // clear buffer for next file
    }
    return(0);
}
