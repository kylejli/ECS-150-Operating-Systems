wcat: "wisconsin" cat
- prints out content of given file
- Usage: ./wcat file.name

wgrep: "wisconsin" grep
- searches file line by line for given search term
- prints out the line if search term found
- Usage: ./wgrep searchterm file.name

wzip: "wisconsin" zip
- compresses files
- compression based on repeating characters
- Format: 4-byte int (count of repeating char) followed by the char
- Usage: ./wzip file.name > output.name

wunzip: "wisconsin" unzip
- decompresses files
- undos wzip
- Usage: ./wunzip file.name

Professor: Samuel King
