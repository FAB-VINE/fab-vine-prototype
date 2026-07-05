# Fab Vine Node V4 Firmware

Status: Sprint 5 workspace / next firmware iteration
Base: V3 firmware
Microcontroller: Seeed Studio XIAO ESP32-C6

V4 is the firmware space for the new sprint. It starts from the current V3 node program so the next sprint can evolve the interface, face-recognition behavior, LED breathing, and board-specific pin mapping without overwriting the Sprint 4 integration reference.

## Files

- `V4.ino` - main Arduino sketch for Sprint 5 work.
- `src/PacketCommunication.h` - packet framing, checksum, send, and receive helper copied from V3.

## Starting Point

This version inherits:

- six-face packet communication;
- OLED eye interface;
- subtle face connection markers;
- 40 NeoPixel LED breathing;
- V3 packet state sharing and neighbor timeout behavior.

## Sprint 5 Notes

Use this folder for code experiments that belong to the new sprint. Keep V3 as the stable Sprint 4 reference, and move confirmed Sprint 5 changes into this folder.

Related web manual:

- `MANUALS/node-sprint5.html`
