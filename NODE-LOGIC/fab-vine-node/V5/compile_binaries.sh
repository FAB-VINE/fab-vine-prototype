#!/bin/bash

# CONFIGURATION
FQBN="esp32:esp32:XIAO_ESP32C6:UploadSpeed=921600,CDCOnBoot=cdc,CPUFreq=160,FlashFreq=80,FlashMode=qio,PartitionScheme=default,DebugLevel=none,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default"
PORT_1="/dev/ttyACM0"
DEFINES_1="-D IS_SEED=true"
# Label for Device 1 (used in filename)
LABEL_1="regular" 

PORT_2="/dev/ttyACM1"
DEFINES_2="-D IS_MIRROR=true -D IS_SEED=false"
# Label for Device 2 (used in filename)
LABEL_2="mirror"

# Output Directory
OUTPUT_DIR="./binaries"

# Helper function to append flags
append_flags() {
    local new_defs="$1"
    local raw_output
    raw_output=$(arduino-cli board details -b "$FQBN" 2>/dev/null)
    
    # Extract current flags
    local current_flags
    current_flags=$(echo "$raw_output" | grep "^compiler.cpp.extra_flags" | awk -F'=' '{print $2}' | xargs)
    
    if [ -z "$current_flags" ]; then
        echo "$new_defs"
    else
        echo "$current_flags $new_defs"
    fi
}

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Get the sketch name (assumes the .ino file in the current directory)
# This grabs the name of the .ino file without the extension
SKETCH_NAME=$(ls *.ino 2>/dev/null | head -n 1 | sed 's/\.ino$//')

if [ -z "$SKETCH_NAME" ]; then
    echo "Error: No .ino file found in the current directory."
    exit 1
fi

echo "Starting automated flashing sequence for ESP32-C6..."
echo "Sketch detected: $SKETCH_NAME"
echo "Output folder: $OUTPUT_DIR"

# --- Device 1 ---
echo "----------------------------------------"
echo "Compiling for Device 1 ($LABEL_1)..."
FINAL_FLAGS_1=$(append_flags "$DEFINES_1")

# Compile and export binary
# --export-binaries ensures the .bin is saved in the sketch folder
arduino-cli compile --fqbn "$FQBN" --build-property "compiler.cpp.extra_flags=$FINAL_FLAGS_1" --export-binaries

if [ $? -eq 0 ]; then
    # The binary is usually named <sketch_name>.ino.bin or <sketch_name>.bin in the sketch folder
    # We need to find the newly created .bin file
    SOURCE_BIN=$(ls build/esp32.esp32.XIAO_ESP32C6/${SKETCH_NAME}*.bin 2>/dev/null | head -n 1)
    
    if [ -n "$SOURCE_BIN" ]; then
        TARGET_BIN="${OUTPUT_DIR}/firmware_ESP32_${LABEL_1}.bin"
        mv "$SOURCE_BIN" "$TARGET_BIN"
        echo "Device 1 compiled. Binary saved as: $TARGET_BIN"
    else
        echo "Error: Compiled binary not found after export."
        exit 1
    fi
else
    echo "Device 1 compilation failed."
    exit 1
fi

# --- Device 2 ---
echo "----------------------------------------"
echo "Compiling for Device 2 ($LABEL_2)..."
FINAL_FLAGS_2=$(append_flags "$DEFINES_2")

# Compile and export binary
arduino-cli compile --fqbn "$FQBN" --build-property "compiler.cpp.extra_flags=$FINAL_FLAGS_2" --export-binaries

if [ $? -eq 0 ]; then
    SOURCE_BIN=$(ls build/esp32.esp32.XIAO_ESP32C6/${SKETCH_NAME}*.bin 2>/dev/null | head -n 1)
    
    if [ -n "$SOURCE_BIN" ]; then
        TARGET_BIN="${OUTPUT_DIR}/firmware_ESP32_${LABEL_2}.bin"
        mv "$SOURCE_BIN" "$TARGET_BIN"
        echo "Device 2 compiled. Binary saved as: $TARGET_BIN"
    else
        echo "Error: Compiled binary not found after export."
        exit 1
    fi
else
    echo "Device 2 compilation failed."
    exit 1
fi

echo "----------------------------------------"
echo "Binaries available in: $OUTPUT_DIR"

