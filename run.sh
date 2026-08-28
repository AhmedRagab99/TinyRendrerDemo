set -e
#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# 1. Define the build directory name
BUILD_DIR="build"

# 2. Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# 3. Configure the project (Generate build files)
echo "Configuring project with CMake..."
cmake -B "$BUILD_DIR" -S . -DCMAKE_BUILD_TYPE=Release

# 4. Compile the project
echo "Building project..."
cmake --build "$BUILD_DIR" --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)


echo "Build completed successfully!"


echo "Running the application..."
# 5. Run the application (assuming the executable is named 'my_app')
"$BUILD_DIR/SoftwareTinyRender"
echo "Finished running app"

