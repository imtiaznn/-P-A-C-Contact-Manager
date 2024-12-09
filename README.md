# C-Contact-Manager

## Installation
NOTE: Ensure that C Compiler - GCC version 9.3.0 or higher (recommended: GCC 11.0+) is
installed in your machine before running the program

# Installation of GCC
To verify GCC is installed and check its version, run this command
in your workplace’s terminal:

```bash
gcc --version

# If it’s not installed:
- Windows: Install MinGW
- Linux: Use your package manager to install
sudo apt update && sudo apt install build-essential
- macOS: Install Xcode Command Line Tools and uses clang to compile the code
xcode-select –install

# Clone the repository:
git clone https://github.com/imtiaznn/C-Contact-Manager.git
# Change directory:
cd C-Contact-Manager

# Compiling the source files using GCC Compiler
gcc main.c file_io.c utils.c -o app.exe

# If you have any errors with math.h functions use the following to compile
gcc main.c file_io.c utils.c -o app.exe  -lm

# Run the program
./app.exe
