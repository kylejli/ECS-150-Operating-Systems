//
//  wzip.cpp
//  
//
//  Created by Kyle on 4/8/21.
//

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <cstdint>
using namespace std;

// wzip() Compresses Given Input(s) Based on Repeating Chars
int main(int argc, char *argv[]) {
    // No File Arguments
    if (argc == 1) {
        cout << "wzip: file1 [file2 ...]" << endl;
        return(1);
    }
    // First File Input
    int fd[argc - 1];
    fd[0] = open(argv[1], O_RDONLY);
    // Check if Open Successful
    if (fd[0] < 0) {
        cout << "wzip: cannot open file" << endl;
        exit(1);
    }
    uint32_t outInt = 1;    // number of repeats of current character
    char outChar;           // current character
    char tempChar;
    size_t bytesRead;
    bytesRead = read(fd[0], &tempChar, 1);
    if (bytesRead > 0) {
        outChar = tempChar;                         // initialize first character
    }
    while (bytesRead > 0) {
        bytesRead = read(fd[0], &tempChar, 1);
        if (bytesRead > 0) {
            // If New Character, Write Previous Int and Char
            if (tempChar != outChar) {
                write(STDOUT_FILENO, &outInt, 4);   // write count of char
                write(STDOUT_FILENO, &outChar, 1);  // write char
                // Update Current Char and Count
                outChar = tempChar;
                outInt = 1;
            }
            else {
                outInt++;                           // increment count of current char
            }
        } // don't reset counter, may repeat character on next file
    }
    // Other File Inputs (3rd Argument and Up)
    for (int i = 2; i < argc; i++) {
        fd[i] = open(argv[i], O_RDONLY);
        // Check If Open Successful
        if (fd[i] < 0) {
            cout << "wzip: cannot open file" << endl;
            exit(1);
        }
        
        do {
            bytesRead = read(fd[i], &tempChar, 1);
            if (bytesRead > 0) {
                if (tempChar != outChar) {
                    write(STDOUT_FILENO, &outInt, 4);
                    write(STDOUT_FILENO, &outChar, 1);
                    outChar = tempChar;
                    outInt = 1;
                }
                else {
                    outInt++;
                }
            }
            
        }
        while (bytesRead > 0);
    }
    // Write Last Character (if File Didn't End with End of Line Char)
    write(STDOUT_FILENO, &outInt, 4);
    write(STDOUT_FILENO, &outChar, 1);
    return(0);
}
