//
//  wcat.cpp
//  wcat
//
//  Created by Kyle on 4/8/21.
//

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

// wcat() Writes Out Contents of Input File(s)
int main(int argc, char *argv[]) {
    // Return 0 if No Input File
    if (argc == 1)
        return(0);
    char buffer[100]; // buffer of 100 chars, read() will read 100 characters at a time
    size_t bytesRead;
    int fileDescriptor;
    // For Input Files (2nd argument and up)
    for (int i = 1; i < argc; i++) {
        fileDescriptor = open(argv[i], O_RDONLY); // open input file
        // Check if Open Successful
        if (fileDescriptor < 0) {
            cout << "wcat: cannot open file" << endl;
            exit(1);
        }
        // Attempt to Read at Least Once, Don't Repeat if No Bytes Read
        do {
            bytesRead = read(fileDescriptor, buffer, 100);
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        while (bytesRead > 0);
        close(fileDescriptor);
    }
    return(0);
}
