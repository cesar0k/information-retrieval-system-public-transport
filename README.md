# My Terrible C++ Project for a course project // Public transport information retrieval system

## Prerequisites

1.  **A C++ Compiler:** Like g++ on Linux, or the one that comes with Visual Studio on Windows.
2.  **CMake:** You can download it from [cmake.org](https://cmake.org/download/).
3.  **SFML (Version 2.6.2 or older):** This project requires a version of SFML prior to 3.0 due to syntax changes. You can find older versions on the [SFML download page] I used version 2.6.2 (https://www.sfml-dev.org/download/sfml/2.6.2).

    *   **On macOS (Manual Install):**
        1.  Download the "macOS clang" version from the website (e.g., 2.6.2).
        2.  Unzip the downloaded file.
        3.  Drag the files from directory Frameworks into the `/Library/Frameworks` directory. You may need to authenticate with your password.

    *   **On Windows:** Download the appropriate package for your compiler (e.g., Visual C++ version 2.6.x) from the SFML website and follow the setup instructions in the "How to Run" section below.

## How to Run

### macOS

1.  **Open your terminal and navigate to the project directory.**
2.  **Create a build directory (if it doesn't exist):**
    ```bash
    mkdir build
    ```
3.  **Navigate into the build directory:**
    ```bash
    cd build
    ```
4.  **Run CMake:**
    ```bash
    cmake ..
    ```
5.  **Compile the project:**
    ```bash
    cmake --build .
    ```
6.  **Run the executable:**
    *   On **Linux**, you'll probably find the executable in the `build` directory: `./Kursach`
    *   On **macOS**, it creates an app bundle. You can run it from the `build` directory: `./Kursach.app/Contents/MacOS/Kursach` or just double-click the `Kursach.app` file.

### Windows

#### 1. Generate the Visual Studio Solution

1.  **Make sure you have a C++ compiler, CMake, and SFML installed.** (Visual Studio with the "Desktop development with C++" workload is the recommended choice).
2.  **Open a terminal (like PowerShell or CMD) and navigate to the project directory.**
3.  **Create a build directory:**
    ```bash
    mkdir build
    ```
4.  **Navigate into the build directory:**
    ```bash
    cd build
    ```
5.  **Run CMake to generate a Visual Studio Solution (`.sln`) file.**

    **Important Note:** CMake cannot find SFML automatically, you must specify the path manually using the `-D SFML_DIR` flag. This path should point to the `lib/cmake/SFML` directory within your SFML installation folder.

    Run one of the following commands:
    ```bash
    # Example if CMake can't find SFML (replace with your actual path)
    cmake .. -D SFML_DIR="C:/SFML-2.6.2/lib/cmake/SFML"

    # Or use this if SFML is in a standard location
    cmake ..
    ```

#### 2. Build and Run in Visual Studio

1.  **Open the `.sln` file** (e.g., `Kursach.sln`) from the `build` directory in Visual Studio.
2.  **Set the startup project.** In the "Solution Explorer" panel, right-click on the **`Kursach`** project (not `ALL_BUILD`) and select **"Set as StartUp Project"**. This tells Visual Studio which program to run.
3.  **Copy SFML DLLs.** Before running, you must copy the SFML library files (`.dll`) into the executable's directory.
    *   Go to your SFML installation folder and open the `bin` directory.
    *   Copy all the `.dll` files.
    *   Paste them into the directory containing your executable, which will be inside the `build` directory (e.g., `build/Debug` or `build/Release`).
4.  **Run the project** from within Visual Studio by pressing `F5` or selecting `Debug > Start Debugging`.

## Disclaimer
Please don't judge me for this code.
