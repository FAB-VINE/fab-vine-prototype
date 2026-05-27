# Contribution and Documentation Manual

Fab Vine welcomes Fab Labs, makers, designers, engineers, artists, students, and researchers.

This manual explains how to document work in the repository and where each type of file should live.

Nota en español: este documento define como colaborar, documentar y ubicar archivos dentro del repositorio Fab Vine.

## Documentation Language

- Use English as the primary language for repository files, commit messages, folder names, and technical documentation.
- Spanish can be included as a secondary note when it helps local coordination, workshops, or field documentation.
- Prefer clear, direct writing over internal jargon.
- If a document is bilingual, place English first and Spanish second.

## General Documentation Rules

- Every important contribution should include a short explanation of what it is, why it exists, and how to use it.
- Prefer Markdown files (`.md`) for manuals, notes, decisions, guides, and project explanations.
- Use descriptive file names in lowercase with hyphens, for example `sensor-calibration.md`.
- Keep source files close to the thing they explain. Do not place hardware notes inside software folders or server notes inside manuals unless they are user-facing guides.
- Add dates when documenting experiments, tests, or decisions that may change later.
- Do not upload temporary exports, duplicated downloads, personal notes, or uncompressed work files unless they are required for the project.

## What Goes in Each Folder

### `MANUALS/`

User-facing and contributor-facing documentation.

Use this folder for:

- Getting started guides.
- Assembly instructions.
- Fabrication and design rules.
- Build steps for a first node.
- Workshop documentation.
- Photos or references used to explain a build process.

Do not place raw CAD, firmware, or server code here. Link to the relevant technical folder instead.

### `MANUALS/assembly-guides/`

Step-by-step assembly documentation for technicians, labs, and workshop participants.

Use this folder for:

- Photo-based build guides.
- Tool lists.
- Assembly sequences.
- Safety notes.
- Troubleshooting during physical assembly.

### `BUILD-FILES/`

Digital fabrication and hardware production files.

Use this folder for:

- 3D printing files.
- CNC, laser cutting, or fabrication files.
- PCB design files.
- Wiring diagrams.
- Physical module assets.

Each file should include enough context to know material, version, scale, and intended use.

### `BUILD-FILES/3d-prints/`

Files for 3D printed or digitally fabricated parts.

Use this folder for:

- STL, OBJ, STEP, or source design files.
- Module shells.
- Node enclosures.
- Connectors and snap-fit parts.

### `BUILD-FILES/3d-prints/cases/`

Enclosures and protective cases for nodes, electronics, batteries, or modules.

### `BUILD-FILES/3d-prints/mounts/`

Wall mounts, brackets, installation supports, clips, and physical attachment parts.

### `BUILD-FILES/pcb-designs/`

PCB source files and board documentation.

Use this folder for:

- KiCad or Eagle projects.
- Schematics.
- Board layouts.
- Gerbers.
- Bill of materials files.

Each board should include a README explaining purpose, version, required components, and known constraints.

### `BUILD-FILES/pcb-designs/sense-board/`

Files for the sensing board, the "brain" or input side of a node.

### `BUILD-FILES/pcb-designs/display-board/`

Files for the light or output board, including LED control and display electronics.

### `BUILD-FILES/wiring-diagrams/`

Visual guides for connecting boards, sensors, LEDs, power, and communication lines.

Use this folder for:

- Pinout diagrams.
- Power diagrams.
- Cable routing.
- Node-to-node connection diagrams.

### `NODE-LOGIC/`

Firmware and code that runs on physical nodes.

Use this folder for:

- Microcontroller code.
- Shared firmware helpers.
- Sensor behavior.
- LED behavior.
- Build and flashing scripts for firmware.

Do not place public website code here.

### `NODE-LOGIC/shared-tools/`

Common code used by multiple node types.

Use this folder for:

- Shared constants.
- Communication helpers.
- Sensor utilities.
- LED pattern helpers.

### `NODE-LOGIC/sense-node/`

Firmware for sensing nodes, the "eyes" of the system.

Use this folder for:

- Motion input.
- Audio input.
- Environmental sensors.
- Local interaction detection.

### `NODE-LOGIC/light-node/`

Firmware for light/display nodes, the "voice" of the system.

Use this folder for:

- LED patterns.
- Light behavior.
- Output animations.
- Status feedback.

### `NODE-LOGIC/build-scripts/`

Tools for compiling, flashing, testing, or packaging firmware.

### `BRAIN-SERVER/`

Backend systems, data processing, and coordination logic.

Use this folder for:

- Server-side coordination.
- Data processing.
- APIs.
- MQTT or message broker integrations.
- Analytics logic.

The public landing page does not live here anymore.

### `BRAIN-SERVER/core/`

Main coordinator service.

Use this folder for:

- Node.js services.
- Realtime coordination.
- Event routing.
- MQTT bridge logic.

### `BRAIN-SERVER/data-hub/`

Processing, analytics, and data transformation.

Use this folder for:

- Python data scripts.
- Aggregation logic.
- Experimental analytics.
- Data normalization pipelines.

### `LANDING-PAGE/`

Public Fab Vine website.

Use this folder for:

- The React landing page.
- Public-facing visual storytelling.
- Website components.
- Website styles.
- Deployment build files for the landing.

Do not place firmware, PCB files, or internal server code here.

### `SIMULATION/`

Simulation and testing without physical hardware.

Use this folder for:

- Virtual installation views.
- Fake data generators.
- Prototype testing tools.
- Sandbox experiments for interaction behavior.

### `SIMULATION/virtual-room/`

3D or spatial simulations of the installation.

### `SIMULATION/fake-data/`

Scripts and sample data for testing without real sensors, nodes, or event activity.

### `LAUNCH-TOOLS/`

Operational scripts for deployment, flashing, testing, and maintenance.

Use this folder for:

- Flashing nodes.
- Running local brokers.
- Triggering OTA updates.
- Repeating technical setup tasks.

Scripts should include usage notes at the top of the file or in a nearby README.

### `PROJECT-INFO/`

Project-level information.

Use this folder for:

- Repository overview.
- License.
- Team notes.
- Contribution rules.
- Project governance.

### `.github/workflows/`

Automation and deployment workflows.

Use this folder for:

- GitHub Pages deployment.
- Automated tests.
- Build checks.

Keep these files technical and concise.

## Minimum README Standard

Every major folder should eventually include a `README.md` with:

- Purpose: what the folder is for.
- Contents: what files should go here.
- How to use: build, run, fabricate, assemble, or test instructions.
- Status: draft, prototype, validated, or deprecated.
- Owner or maintainer, when known.

## File Naming

Use:

- `lowercase-with-hyphens.md`
- `module-name-v01.stl`
- `sense-board-v02.kicad_pro`
- `wiring-node-to-led-v01.pdf`

Avoid:

- `final-final-v3-new-copy.file`
- spaces in technical filenames
- unclear names like `test1`, `new`, `copy`, or `untitled`

## Version Notes

Use versions when files may be fabricated, flashed, or shared externally.

Examples:

- `leaf-module-v01.stl`
- `sense-node-firmware-v02`
- `display-board-v03`

For living documents, prefer a changelog section instead of creating many duplicated files.

## Before Adding a New File

Ask:

- Is this source material, documentation, generated output, or temporary work?
- Which folder owns this type of file?
- Will another contributor understand what this is in six months?
- Does the file need a README, photo, diagram, or usage note?
- Is the file small enough and appropriate for Git?

## Do Not Commit

Avoid committing:

- `node_modules/`
- build outputs such as `dist/`
- local environment files
- personal notes
- temporary screenshots
- exported duplicates
- cache folders
- credentials, API keys, or private tokens

## Contribution Workflow

1. Place files in the correct folder.
2. Add or update documentation near the files.
3. Use clear commit messages in English.
4. Verify builds, firmware, or documentation links when relevant.
5. Open a pull request or share the branch with a short summary of what changed.

## Good Commit Message Examples

- `Add first sense node wiring diagram`
- `Document snap-fit design rules`
- `Update landing page deployment workflow`
- `Add MQTT fake data generator`

## Review Checklist

Before requesting review, confirm:

- The file is in the right folder.
- The name is clear.
- Documentation exists or was updated.
- The change does not break the landing page.
- Generated files and local caches are excluded.
- Links use the new repository structure.
