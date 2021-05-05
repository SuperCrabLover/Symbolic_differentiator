# Symbolic_differentiator_ACRONIS
## Contents
0. [Brief description](#Description)
1. [Installation](#Installation)
2. [Usage](#Usage)
## Description
Symbolic differentiator written for admission to the department of MIPT Acronis. Code is powered by the pdf generator written by https://github.com/AndreRenaud/PDFGen. Program parses math expressions in standart notation from a file using recursive descent and builds a special binary tree. Tree is being differentiated and simplified recursively too. Than pdf gen is called and the answer is printed in output.pdf. Author: Molozhavenko Alexander MIPT 2021
## Installation
    git clone
    make
## Usage
After executing make the ELF named 'a' will be created. For getting usage information execute 
    
    ./a -h
The output will be printed into terminal and into the file simultaneously.
