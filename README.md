# Fab Vine Prototype

[![CC BY-NC 4.0][cc-by-nc-shield]][cc-by-nc]

Fab Vine is a living network of fabricated modules, embedded electronics, light, sensing, and community interaction for FAB26. This repository contains the public website, contributor manuals, node firmware, build files, simulation assets, and project documentation for the prototype.

## Public Website

- [Fab Vine public site](https://fab-vine.github.io/fab-vine-prototype/)
- [Manuals library](https://fab-vine.github.io/fab-vine-prototype/MANUALS/index.html)
- [Fab Vine Module V1 BOM](https://fab-vine.github.io/fab-vine-prototype/MANUALS/bom-v1.html)
- [Download the V1 BOM Excel file](https://fab-vine.github.io/fab-vine-prototype/BUILD-FILES/bom/fab-vine-module-v1-bom.xlsx)

## Key Resources

- [Module V1 firmware](NODE-LOGIC/fab-vine-node/V1/firmware/fab-vine-node-v1/fab-vine-node-v1.ino)
- [Node V1 manual](MANUALS/node-v1.html)
- [OLED expressions manual](MANUALS/oled-expressions.html)
- [Design rules](MANUALS/design-rules.html)
- [Landing page source](LANDING-PAGE/)

## Repository Structure

- [MANUALS/](MANUALS/) - contributor manuals, assembly documentation, BOM pages, and hardware notes.
- [BUILD-FILES/](BUILD-FILES/) - fabrication files, PCB design folders, wiring diagrams, and BOM workbook files.
- [NODE-LOGIC/](NODE-LOGIC/) - firmware and shared logic for physical Fab Vine nodes.
- [BRAIN-SERVER/](BRAIN-SERVER/) - server and data-processing experiments.
- [LANDING-PAGE/](LANDING-PAGE/) - source for the public Fab Vine website.
- [SIMULATION/](SIMULATION/) - virtual installation and fake-data tools.
- [LAUNCH-TOOLS/](LAUNCH-TOOLS/) - scripts and utilities for flashing, testing, and deployment.
- [PROJECT-INFO/](PROJECT-INFO/) - project metadata, contribution notes, team information, and license details.

## Current Hardware Focus

The current V1 module direction uses a Seeed Studio XIAO ESP32-C6, an I2S MEMS microphone, a 128x64 I2C OLED display, 40 addressable LEDs per module, Micro-Fit power/signal wiring, and shared 5V power supplies for a 60-module installation.

The current firmware MVP is offline and social: each node can detect up to six physical neighbor faces, count occupied faces from 0 to 6, and react with OLED expressions plus zig-zag mapped LED behavior. When alone, the module breathes purple; when one or more neighbors are detected, connected face positions glow green/blue with no red channel.

## License

This work is licensed under the [Creative Commons Attribution-NonCommercial 4.0 International License][cc-by-nc].

[![CC BY-NC 4.0][cc-by-nc-image]][cc-by-nc]

[cc-by-nc]: https://creativecommons.org/licenses/by-nc/4.0/
[cc-by-nc-image]: https://licensebuttons.net/l/by-nc/4.0/88x31.png
[cc-by-nc-shield]: https://img.shields.io/badge/License-CC%20BY--NC%204.0-lightgrey.svg
