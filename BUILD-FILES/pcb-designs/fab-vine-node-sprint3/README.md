# Fab Vine Node Sprint 3 PCB

Status: Ready to fabricate  
Package date: 2026-06-01  
Board target: Fab Vine node PCB for Seeed Studio XIAO ESP32-C6

This folder contains the Sprint 3 PCB package received from the board team. It is organized as the active fabrication package for the next hardware direction.

## Files

- `gerbers/Gerber_FabVine_PCB_FabVine_2026-06-01.zip` - fabrication Gerber package.
- `previews/pcb-layout-top.jpeg` - top copper/layout preview.
- `previews/pcb-layout-bottom.jpeg` - bottom copper/layout preview.
- `previews/pcb-3d-top.jpeg` - top 3D preview.
- `previews/pcb-3d-bottom.jpeg` - bottom 3D preview.
- `3d-model/OBJ_PCB_FabVine.obj` - PCB 3D model geometry.
- `3d-model/OBJ_PCB_FabVine.mtl` - material file used by the PCB 3D model.
- `reference/fab-vine-xiao-esp32c6-pinout-sprint3.xlsx` - Fab Vine Sprint 3 pinout workbook.
- `reference/XIAO_ESP32C6_Pinout.xlsx` - official Seeed XIAO ESP32-C6 pinout sheet.

## Sprint 3 Pinout Direction

The Sprint 3 PCB direction changes the neighbor connector logic from detect-to-ground to active TX/RX signaling.

| Function | XIAO pin |
|---|---:|
| Neighbor TX common | `D6` |
| Face 1 RX | `D9` |
| Face 2 RX | `D1` |
| Face 3 RX | `D2` |
| Face 4 RX | `D3` |
| Face 5 RX | `D8` |
| Face 6 RX | `D7` |
| OLED SDA | `D4` |
| OLED SCL | `D5` |
| LED data | `D10` |
| Reserved/free | `D0` |

Each face connector should carry:

```text
GND
TX_COMMON = D6
RX_FACE_n = unique face RX pin
```

When two identical PCBs connect face-to-face, the physical connector geometry should produce:

```text
Module A GND <----> Module B GND
Module A TX  ----> Module B RX
Module A RX  <---- Module B TX
```

## Firmware Compatibility Note

The current tested node firmware is still the Sprint 2 prototype. It uses:

- `D8` as the shared data/signal line.
- `D0`, `D1`, `D2`, `D3`, `D6`, `D7` as face detect inputs.
- Detect-to-ground behavior with `INPUT_PULLUP`.

Sprint 3 PCB documentation is the next hardware direction. Firmware must be updated before the Sprint 3 PCB pinout can be tested on the physical modules.
