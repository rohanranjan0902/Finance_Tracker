# FinTrack - Quick Start Guide

Welcome to FinTrack! This guide will help you get the project up and running on your Windows system with VS Code.

## 🚀 Quick Setup (5 minutes)

### Option 1: Automated Setup (Recommended)

1. **Run the setup script**:
   ```cmd
   # Right-click and "Run as Administrator"
   setup_environment.bat
   ```

2. **Open in VS Code**:
   ```cmd
   cd C:\Users\ranja\Desktop\FinTrack
   code .
   ```

3. **Build and run**:
   - Press `Ctrl+Shift+P` → Type "Tasks: Run Task" → Select "CMake: Build"
   - Or use the build script: `build_project.bat`

### Option 2: Manual Setup

1. **Install Visual Studio 2022 Community** (free):
   - Download: https://visualstudio.microsoft.com/downloads/
   - Select "Desktop development with C++" workload
   - Include CMake tools

2. **Install VS Code**:
   - Download: https://code.visualstudio.com/

3. **Install required extensions**:
   - Open VS Code
   - Install: C/C++, CMake Tools

## 🛠️ Building the Project

### Method 1: VS Code Integration
- Open project in VS Code: `code .`
- Press `F1` → "CMake: Configure"
- Press `F1` → "CMake: Build"
- Press `F5` to run with debugging

### Method 2: Command Line
```cmd
# Using the build script
build_project.bat

# Or manually with CMake
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

### Method 3: Direct Compilation (if CMake fails)
```cmd
g++ -std=c++17 -I src src/models/*.cpp src/services/*.cpp src/main.cpp -o build/FinTrack.exe -pthread
```

## ▶️ Running the Application

After building, run:
```cmd
# From project root
build\FinTrack.exe

# Or from VS Code
# Press F5 or Ctrl+F5
```

## 🎯 What You'll See

The application demonstrates:
- ✅ User and account management
- ✅ Thread-safe transaction processing
- ✅ Real-time fraud detection
- ✅ Budget management with alerts
- ✅ Concurrent operations simulation

## 🔧 VS Code Features

Once set up, you'll have:
- **IntelliSense**: Smart code completion
- **Debugging**: Set breakpoints with F9
- **Build Tasks**: Ctrl+Shift+P → "Tasks: Run Task"
- **Error Detection**: Real-time error highlighting
- **Git Integration**: Source control built-in

### Useful VS Code Shortcuts:
- `F5`: Start debugging
- `Ctrl+F5`: Run without debugging
- `Ctrl+Shift+B`: Build task
- `Ctrl+Shift+P`: Command palette
- `F9`: Toggle breakpoint

## 🐛 Troubleshooting

### "CMake not found"
```cmd
# Install via Visual Studio Installer or download from cmake.org
# Make sure it's added to PATH
```

### "Compiler not found"
```cmd
# Option 1: Install Visual Studio with C++ tools
# Option 2: Install MinGW-w64
choco install mingw  # If you have Chocolatey
```

### "Build fails"
```cmd
# Try the manual g++ compilation method
# Check that all .cpp files exist in their correct folders
```

### VS Code IntelliSense issues
- Press `Ctrl+Shift+P`
- Type "C++: Edit Configurations"
- Select your compiler (MSVC or GCC)

## 📁 Project Structure Quick Reference

```
FinTrack/
├── src/
│   ├── models/         # User, Account, Transaction, Budget
│   ├── services/       # TransactionService, FraudDetectionService
│   └── main.cpp        # Application entry point
├── .vscode/            # VS Code configuration
├── build/              # Compiled output
├── CMakeLists.txt      # Build configuration
├── build_project.bat   # Easy build script
└── README.md           # Detailed documentation
```

## 🎮 Demo Features

The application showcases:
1. **Multi-user system** with different account types
2. **Concurrent transactions** with mutex protection
3. **Fraud detection** with configurable rules
4. **Budget tracking** with real-time alerts
5. **Transaction history** and analytics

## ✅ Verification Checklist

- [ ] Project builds without errors
- [ ] Application runs and shows welcome banner
- [ ] All demo features execute successfully
- [ ] VS Code IntelliSense works (hover over variables)
- [ ] Debugging works (can set breakpoints)

## 🚀 Next Steps

1. **Explore the code**: Start with `main.cpp`
2. **Set breakpoints**: Debug the transaction flow
3. **Modify features**: Add new transaction types
4. **Extend fraud rules**: Create custom detection algorithms
5. **Add database**: Integrate SQLite for persistence

## 💡 Tips

- Use `Ctrl+Click` to navigate to function definitions
- The project uses modern C++17 features extensively
- All operations are thread-safe with mutex protection
- The fraud detection runs in a background thread
- Budget alerts trigger automatically when thresholds are reached

---

**Need help?** Check the detailed [README.md](README.md) or the troubleshooting section above.

**Happy coding!** 🎉
