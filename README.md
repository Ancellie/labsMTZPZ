# MTZPZ lab 1
This console application converts Markdown files into HTML fragments. The generated HTML markup can be output to standard output (stdout) or written to an output file if provided with the --out argument. Additionally, the application performs checks for common Markdown errors, such as nested or unbalanced markup tags. If it encounters incorrect Markdown due to nesting issues or unbalanced tags, it will output an error to the standard error (stderr) and terminate with a non-zero exit code.

## Prerequisites:
Does not support Cyrillic

## Installation
Clone the repository from GitHub:

```git clone https://github.com/Ancellie/labsMTZPZ.git```

```cd labsMTZPZ```

## Usage
1. Compile C++ files:
Enter the command to compile your C++ files:

```g++ -o output_file src/main/cpp/*.cpp```

Replace output_file with the name you want for the compiled executable file.

2. Execute the compiled file:
After successful compilation, you can run C++ code:

```./output_file /path/to/markdownfile.txt --out /path/to/output.html```

# Revert commit
https://github.com/Ancellie/labsMTZPZ/commit/2f52ba405c10b608510ae4f03d0700f19ea017c2
