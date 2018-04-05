# C Implementation for hashtable

The program offers a C implementation for the hashtable for storing strings.

## Getting Started

Run the make command to generate the executable file.

Once generated, run the program.

```
$  ./tema1 initial_hashtable_size <input_file>
```
The input file will contain commands that need to be executed on the hashtable.

In case the input_file parameter is missing, the program will read the commands from STDIN.

## Supported commands

* add <string> - Adds a new string in the hashtable. It does not support duplicate values.
* remove <string> - Deletes a string from the hashtable.
* find <string> [<file_name>] - Searches for the string and writes True of False to the console (if no file_name is provided) or to the file.
* clear - Deletes all the entries in the hashtable.
* print - Prints all the entries in the hashtable.
* resize double - Doubles the size of the hasthable.
* resize halve

