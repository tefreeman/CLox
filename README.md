# Lox Interpreter

## Introduction
This README provides instructions for running the Lox Interpreter, a program designed to parse and execute Lox language scripts.

## Testing System

The interpreter incorporates a custom testing framework, as defined in `LoxTest.h` and `LoxTest.cpp`, which assesses the functionality of the interpreter by comparing the expected output with the actual output.
200+ test files provided by https://github.com/munificent/craftinginterpreters/tree/master/test are included. All .lox test files located in the `/tests` folder (and any sub-directory) are processed in test mode. To add a test to the test suite simply drag and drop a .lox file
into the test folder.

- **Requirements:** The tests folder must be located inside the same directory as the binary Lox.exe program.
  
- **Test File Parsing:** The `LoxTest` class parses test files to extract expected outputs from comments. It uses `// expect:` for expected outputs and `// error` for expected errors.

- **Output Validation:** The `IsEquivalent` function evaluates the similarity between expected and actual outputs, covering scenarios like string equivalence, numeric comparisons, and error matching.

- **Test Execution:** Tests are run by creating a `LoxTest` instance with test file content and invoking the `validate` method, which verifies if the interpreter's outputs align with the expected results.

- **Results Reporting:** The `printResults` method reports the outcomes of the tests, highlighting both successful and failed tests.


# Installation and Running Instructions

This document provides detailed instructions on how to install and run the `Lox` application.

## Installation

1. **Download the Repository**
   - First, download the repository containing the necessary files.

2. **Locate the Binary File**
   - Within the downloaded repository, there is a Windows x64 binary file named `Lox.exe`.

## Running the Application

After installation, you can run the `Lox` application in different modes using a terminal such as PowerShell or Command Prompt. Navigate to the directory of the downloaded repository to execute the following commands.


### Interactive Mode

To run the interpreter in interactive mode, execute without any arguments:
```bash
./Lox
```
### Script Mode
```bash
./Lox path/to/your_script.lox
```

### Test Mode 
```bash
./Lox -test
```

## Building Instructions for the Project
 Step-by-step guide on how to build the project using Microsoft Visual Studio.

## Prerequisites

- Microsoft C++/C Compiler (MSVC)
- C++17 Standard

## Steps

1. **Download the Repository**
   - Begin by downloading the repository from the provided link.

2. **Install Microsoft Visual Studio**
   - Download and install Microsoft Visual Studio for Windows or Mac from [Visual Studio's official website](https://visualstudio.microsoft.com/).

3. **Open the Project**
   - After installing Visual Studio, open the project by double-clicking the `.sln` file. This action should load the project along with all the necessary settings.

4. **Build the Project**
   - In the Visual Studio IDE, navigate to the `Build` tab located at the top of the window.
   - Select `Build Solution` from the dropdown menu. This step will compile and build the project.

5. **Locate the Executable**
   - Once the build process is complete, you can find the `Yisp.exe` file in the `Yisp/x64/Debug/` folder.
