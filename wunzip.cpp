//
//  wunzip.cpp
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
#include <string.h>
using namespace std;

// wunzip() Uncompresses Given Input(s) Based on
// Repeating Character Compression done by wzip()
int main(int argc, char *argv[]) {
    // No File Arguments
    if (argc == 1) {
        cout << "wunzip: file1 [file2 ...]" << endl;
        return(1);
    }
    int fd[argc - 1];
    // First File Input
    fd[0] = open(argv[1], O_RDONLY);
    if (fd[0] < 0) {
        cout << "wzip: cannot open file" << endl;
        exit(1);
    }
    uint32_t inInt; // input integer
    char inChar;    // input char
    size_t bytesRead;
    // Read First Int
    bytesRead = read(fd[0], &inInt, 4);
    if (bytesRead > 0) {
        // Read Char if Int Found
        bytesRead = read(fd[0], &inChar, 1);
        for (uint32_t i = 0; i < inInt; i++) {
            // Write Input Char to STDOUT inInt times
            write(STDOUT_FILENO, &inChar, 1);
        }
    }
    // Reads Remaining File
    while (bytesRead > 0) {
        bytesRead = read(fd[0], &inInt, 4);
        if (bytesRead > 0) {
            bytesRead = read(fd[0], &inChar, 1);
            for (uint32_t i = 0; i < inInt; i++) {
                write(STDOUT_FILENO, &inChar, 1);
            }
        }
    }
    // - Other File Inputs
    for (int i = 1; i < argc - 1; i++) {
        fd[i] = open(argv[i + 1], O_RDONLY);
        if (fd[i] < 0) {
            cout << "wzip: cannot open file" << endl;
            exit(1);
        }
        // Read First Int
        bytesRead = read(fd[i], &inInt, 4);
        if (bytesRead > 0) {
            // Read Char if Int Found
            bytesRead = read(fd[i], &inChar, 1);
            // Write Input Char to STDOUT inInt times
            for (uint32_t i = 0; i < inInt; i++) {
                write(STDOUT_FILENO, &inChar, 1);
            }
        }
        // Reads Remaining File
        while (bytesRead > 0) {
            bytesRead = read(fd[i], &inInt, 4);
            if (bytesRead > 0) {
                bytesRead = read(fd[i], &inChar, 1);
                for (uint32_t i = 0; i < inInt; i++) {
                    write(STDOUT_FILENO, &inChar, 1);
                }
            }
        }
    }
    return(0);
}
