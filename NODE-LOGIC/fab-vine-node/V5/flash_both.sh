#!/bin/bash

# CONFIGURATION
FQBN="esp32:esp32:XIAO_ESP32C6:UploadSpeed=921600,CDCOnBoot=cdc,CPUFreq=160,FlashFreq=80,FlashMode=qio,PartitionScheme=default,DebugLevel=none,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default" # Update if your board ID differs
PORT_1="/dev/ttyACM0"
DEFINES_1="-D IS_SEED=true"

PORT_2="/dev/ttyACM1"
DEFINES_2="-D IS_MIRROR=true -D IS_SEED=false"


append_flags() {
    local new_defs="$1"
        
    local raw_output
    raw_output=$(arduino-cli board details -b "$FQBN" 2>/dev/null)
    
    # 2. Extract the current value of compiler.cpp.extra_flags
    local current_flags
    current_flags=$(echo "$raw_output" | grep "^compiler.cpp.extra_flags" | awk -F'=' '{print $2}' | xargs)
    
    # 3. Construct the final string
    if [ -z "$current_flags" ]; then
        echo "$new_defs"
    else
        echo "$current_flags $new_defs"
    fi
}

echo "Starting automated flashing sequence for ESP32-C6..."

echo "Compiling for Device 1..."
FINAL_FLAGS_1=$(append_flags "$DEFINES_1")

# Debug: Uncomment to see exactly what flags are being passed
# echo "Flags for Device 1: $FINAL_FLAGS_1"

arduino-cli compile --fqbn "$FQBN" --build-property "compiler.cpp.extra_flags=$FINAL_FLAGS_1"

if [ $? -eq 0 ]; then
    echo "Device 1 compiled. Uploading to $PORT_1..."
    arduino-cli upload -p "$PORT_1" --fqbn "$FQBN"
else
    echo "Device 1 compilation failed."
    exit 1
fi

# --- Device 2: IS_MIRROR = false ---

echo "Compiling for Device 2..."
FINAL_FLAGS_2=$(append_flags "$DEFINES_2")

# Debug: Uncomment to see exactly what flags are being passed
# echo "Flags for Device 2: $FINAL_FLAGS_2"

arduino-cli compile --fqbn "$FQBN" --build-property "compiler.cpp.extra_flags=$FINAL_FLAGS_2"

if [ $? -eq 0 ]; then
    echo "Device 2 compiled. Uploading to $PORT_2..."
    arduino-cli upload -p "$PORT_2" --fqbn "$FQBN"
else
    echo "Device 2 compilation failed."
    exit 1
fi

echo "Both devices flashed successfully!"