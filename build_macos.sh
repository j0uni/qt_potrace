#!/bin/bash
set -e  # Exit on error

# Function to find Qt installation
find_qt() {
    # First try to find Qt in home directory
    echo "Searching Qt in home directory..." >&2
    
    # Common Qt installation paths in home
    local qt_home_paths=(
        "$HOME/Qt"
        "$HOME/Applications/Qt"
    )
    
    for base_path in "${qt_home_paths[@]}"; do
        if [ -d "$base_path" ]; then
            echo "Found Qt base directory: $base_path" >&2
            
            # Find all version directories
            while IFS= read -r version_dir; do
                echo "Checking version directory: $version_dir" >&2
                
                # Check common paths within version directory
                local macos_paths=(
                    "$version_dir/macos"
                    "$version_dir/clang_64"
                    "$version_dir/gcc_64"
                )
                
                for macos_path in "${macos_paths[@]}"; do
                    if [ -f "$macos_path/bin/qmake" ]; then
                        echo "Found valid Qt installation at: $macos_path" >&2
                        echo "$macos_path"
                        return 0
                    fi
                done
            done < <(find "$base_path" -maxdepth 1 -type d -name "[0-9]*.[0-9]*.[0-9]*" | sort -rV)
        fi
    done
    
    # If not found in home, try system locations
    echo "Searching Qt in system locations..." >&2
    local system_paths=(
        "/opt/homebrew/opt/qt"
        "/usr/local/opt/qt"
        "/opt/qt"
        "/Applications/Qt"
    )
    
    for path in "${system_paths[@]}"; do
        echo "Checking system path: $path" >&2
        if [ -d "$path" ] && [ -f "$path/bin/qmake" ]; then
            echo "Found valid Qt installation at: $path" >&2
            echo "$path"
            return 0
        fi
    done
    
    echo "Qt installation not found!" >&2
    exit 1
}

# Main script
echo "Finding Qt installation..."
QT_PATH=$(find_qt)
echo "Found Qt at: $QT_PATH"

# Add Qt to PATH and set QMAKESPEC
export PATH="$QT_PATH/bin:$PATH"
export QMAKESPEC="$QT_PATH/mkspecs/macx-clang"

# Clean build directory
echo "Cleaning build directory..."
rm -rf build
mkdir build
cd build

# Run qmake and make
echo "Building application..."
qmake ..
make

# Check if build was successful
if [ ! -f "qt_potrace.app/Contents/MacOS/qt_potrace" ]; then
    echo "Build failed! Application not found."
    exit 1
fi

# Kill any existing instances
pkill -x qt_potrace || true

# Start the app
echo "Starting app..."
open qt_potrace.app

echo "Build completed successfully!" 