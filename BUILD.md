# Building FinTrack

## Prerequisites

- **C++ Compiler**: GCC 7.3+, MSVC 2019+, or Clang 6.0+
- **CMake**: Version 3.16 or higher
- **Git**: For version control

## Build Instructions

### Windows (Visual Studio)

1. **Open Command Prompt or PowerShell**

2. **Navigate to the project directory:**
   ```cmd
   cd C:\Users\PRATIK\OneDrive\Desktop\FinTrack
   ```

3. **Create and enter build directory:**
   ```cmd
   mkdir build
   cd build
   ```

4. **Generate build files:**
   ```cmd
   cmake ..
   ```

5. **Build the project:**
   ```cmd
   cmake --build . --config Release
   ```

6. **Find your executable:**
   ```cmd
   cd bin\Release
   FinTrack.exe
   ```

### Windows (MinGW)

1. **Open Command Prompt**

2. **Navigate and build:**
   ```cmd
   cd C:\Users\PRATIK\OneDrive\Desktop\FinTrack
   mkdir build && cd build
   cmake .. -G "MinGW Makefiles"
   mingw32-make
   ```

3. **Run:**
   ```cmd
   cd bin
   FinTrack.exe
   ```

### Linux/macOS

1. **Open Terminal**

2. **Navigate and build:**
   ```bash
   cd ~/FinTrack  # or your project location
   mkdir build && cd build
   cmake ..
   make -j4
   ```

3. **Run:**
   ```bash
   cd bin
   ./FinTrack
   ```

## Build Types

### Debug Build (default)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Release Build (optimized, recommended for deployment)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Creating a Portable Executable

The CMakeLists.txt is configured for static linking to create a more portable executable.

### Windows
The MSVC runtime is statically linked, creating a standalone `.exe` file.

### Linux/macOS
GCC/Clang standard libraries are statically linked where possible.

## Deployment

### Single File Distribution

After building in Release mode, you can distribute just the executable:

**Windows:**
```
FinTrack/build/bin/Release/FinTrack.exe
```

**Linux/macOS:**
```
FinTrack/build/bin/FinTrack
```

### Package Structure (Recommended)

For a complete distribution, include:

```
FinTrack-v1.0/
├── FinTrack.exe (or FinTrack on Unix)
├── USER_GUIDE.md
└── README.md
```

## Troubleshooting

### Windows: Missing DLL errors
- Make sure you built in Release mode with static linking
- If using MinGW, ensure MinGW is in your PATH

### Linux: Permission denied
```bash
chmod +x FinTrack
```

### CMake errors
- Verify CMake version: `cmake --version`
- Clear build directory and retry: `rm -rf build && mkdir build`

### Compiler not found
- **Windows**: Install Visual Studio Build Tools or MinGW
- **Linux**: Install build-essential: `sudo apt-get install build-essential cmake`
- **macOS**: Install Xcode Command Line Tools: `xcode-select --install`

## Testing the Build

After building, test the application:

1. Run the executable
2. Create a test user
3. Create an account
4. Perform a deposit
5. Check transaction history
6. Exit cleanly

## Clean Build

To start fresh:

```bash
# Remove build directory
rm -rf build

# Or on Windows PowerShell
Remove-Item -Recurse -Force build
```

## Advanced Options

### Static Analysis
If you have clang-tidy installed:
```bash
cmake .. -DCMAKE_CXX_CLANG_TIDY=clang-tidy
cmake --build .
```

### Verbose Build
```bash
cmake --build . --verbose
```

### Parallel Build
```bash
make -j8  # Use 8 parallel jobs
```

## Performance Notes

- Release builds are ~10x faster than Debug builds
- Static linking adds ~2-5MB to executable size
- Initial compilation takes 30-60 seconds
- Incremental builds take 5-10 seconds

## Next Steps

After building successfully:
1. Read USER_GUIDE.md for usage instructions
2. Test all features thoroughly
3. Report any issues or bugs
4. Consider adding database persistence (see SQLITE_INTEGRATION.md)

---

**Questions?** Check the project README.md or create an issue.

