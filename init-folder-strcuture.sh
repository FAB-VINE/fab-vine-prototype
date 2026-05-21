#!/bin/bash

# 🌿 Fab-Vine Repository Initializer
# Creates the collaborative structure with .gitkeep files for empty folders.
# Does NOT commit changes automatically.

echo "🌱 Initializing Fab-Vine Structure..."

# 1. Define the directory tree
# Using braces to create nested directories in one go
DIRS=(
  "MANUALS/sprint-1-seed"
  "MANUALS/sprint-2-branch"
  "MANUALS/sprint-3-trunk"
  "MANUALS/sprint-4-canopy"
  "MANUALS/sprint-5-forest"
  "MANUALS/assembly-guides"
  "MANUALS/design-rules"
  
  "BUILD-FILES/3d-prints/cases"
  "BUILD-FILES/3d-prints/mounts"
  "BUILD-FILES/3d-prints/universal-joint"
  "BUILD-FILES/pcb-designs/sense-board"
  "BUILD-FILES/pcb-designs/display-board"
  "BUILD-FILES/wiring-diagrams"
  
  "NODE-LOGIC/shared-tools"
  "NODE-LOGIC/sense-node/src"
  "NODE-LOGIC/light-node/src"
  "NODE-LOGIC/build-scripts"
  
  "BRAIN-SERVER/core"
  "BRAIN-SERVER/data-hub"
  "BRAIN-SERVER/dashboard"
  
  "SIMULATION/virtual-room"
  "SIMULATION/fake-data"
  
  "LAUNCH-TOOLS"
  
  "PROJECT-INFO"
  
  ".github/workflows"
)

# 2. Create directories
for dir in "${DIRS[@]}"; do
  mkdir -p "$dir"
done

# 3. Create .gitkeep files in ALL directories (except root)
# This ensures empty folders are tracked by git
find . -type d -name ".git" -prune -o -type d -exec touch {}/.gitkeep \;


# Project Info README
cat > PROJECT-INFO/README.md <<EOF
# 📦 Project Overview

## Roles & Responsibilities
- **Artists:** Work in `BUILD-FILES/3d-prints` and `SIMULATION`.
- **Technicians:** Follow guides in `MANUALS` and use `LAUNCH-TOOLS`.
- **Developers:** Code in `NODE-LOGIC` and `BRAIN-SERVER`.

## The Sprints
1. **Seed:** Proof of Concept (Sprint 1)
2. **Branch:** Snap-Mechanism (Sprint 2)
3. **Trunk:** Vertical Stack (Sprint 3)
4. **Canopy:** Full Tree (Sprint 4)
5. **Forest:** 50-Node Scale (Sprint 5)
EOF

# Sprint 1 Manual Placeholder
cat > MANUALS/sprint-1-seed/README.md <<EOF
# 🌱 Sprint 1: The Seed

## Goal
Connect a sensor to a light via MQTT.

## Tasks
- [ ] Wire ESP32 Dev Board
- [ ] Connect test Sensor
- [ ] Connect test LED Strip
- [ ] Flash `sense-node` and `light-node`
- [ ] Verify data flow in `BRAIN-SERVER`
EOF

# Sprint 2 Manual Placeholder
cat > MANUALS/sprint-2-branch/README.md <<EOF
# 🌿 Sprint 2: The Branch

## Goal
Design and test the Universal Snap Joint.

## Tasks
- [ ] Finalize 3D print of `BUILD-FILES/3d-prints/universal-joint`
- [ ] Test electrical continuity across snap
- [ ] Implement "Neighbor Detection" in firmware
EOF

# LAUNCH-TOOLS Placeholder
cat > LAUNCH-TOOLS/README.md <<EOF
# 🚀 Launch Tools

Scripts to run, flash, and update the system.

## Available Scripts
- `flash-nodes.sh`: Flash firmware to local nodes.
- `start-broker.sh`: Start local MQTT broker.
- `update-all.sh`: OTA update for production.
EOF

# .gitignore
cat > .gitignore <<EOF
# OS
.DS_Store
Thumbs.db

# Logs
*.log

# Build outputs
BUILD-FILES/pcb-designs/*/build/
NODE-LOGIC/*/build/

# IDE
.vscode/
.idea/
EOF

# 5. Summary
echo "✅ Structure created successfully!"
echo ""
echo "📂 Generated Directories:"
find . -type d | grep -v ".git" | sort
echo ""
echo "📝 Created Files:"
find . -type f | grep -v ".git" | sort
echo ""
echo "💡 Next Step: Review the files above, then run 'git add .' and 'git commit -m \"Initial Fab-Vine Structure\"'"