# C-Contact-Manager

NOTE: Ensure that C Compiler - GCC version 9.3.0 or higher (recommended: GCC 11.0+) is
installed in your machine before running the program

## Installation of GCC
To verify GCC is installed and check its version, run this command
in your workplace’s terminal:

```bash
gcc --version
```


If it’s not installed:
- Windows: Install MinGW
- Linux: Use your package manager to install
```bash
sudo apt update && sudo apt install build-essential
```
- macOS: Install Xcode Command Line Tools and uses clang to compile the code
```bash
xcode-select –install
```

## Getting the repository:
```bash
git clone https://github.com/imtiaznn/C-Contact-Manager.git
```

Change directory:
```bash
cd C-Contact-Manager
```

Compile the source files using GCC Compiler
```bash
gcc main.c file_io.c utils.c -o app.exe
```

If you have any errors with math.h functions use the following to compile
```bash
gcc main.c file_io.c utils.c -o app.exe  -lm
```

Run the program
```bash
./app.exe
```