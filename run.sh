#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# 1. Define the build directory name
BUILD_DIR="build"
CLEAN=0

for arg in "$@"; do
  case "$arg" in
    --clean|-c)
      CLEAN=1
      ;;
    -h|--help)
      echo "Usage: $0 [--clean|-c]"
      echo "  --clean, -c   Remove the existing build directory (CMake cache included) before building"
      exit 0
      ;;
    *)
      echo "Unknown argument: $arg" >&2
      echo "Usage: $0 [--clean|-c]" >&2
      exit 1
      ;;
  esac
done

# 2. Remove the build directory first if --clean was requested
if [ "$CLEAN" -eq 1 ]; then
    echo "Removing existing build cache..."
    rm -rf "$BUILD_DIR"
fi

# 3. Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# 4. Configure the project (Generate build files)
echo "Configuring project with CMake..."
cmake -B "$BUILD_DIR" -S . -DCMAKE_BUILD_TYPE=Release

# 5. Compile the project
echo "Building project..."
cmake --build "$BUILD_DIR" --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)


echo "Build completed successfully!"


echo "Running the application..."
# 6. Run the application
"$BUILD_DIR/SoftwareTinyRender"
echo "Finished running app"
