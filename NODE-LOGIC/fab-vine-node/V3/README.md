# Fab Vine Node V3 Firmware

Status: active integration firmware  
Board target: Sprint 3 PCB direction / Sprint 4 integration documentation  
Microcontroller: Seeed Studio XIAO ESP32-C6

V3 updates the node program for the active TX/RX face-connector direction. It sends a shared packet through `D6`, listens to six face-specific RX pins, updates an OLED eye interface, and drives a 40-pixel NeoPixel strip.

## Files

- `V3.ino` - main Arduino sketch.
- `src/PacketCommunication.h` - packet framing, checksum, send, and receive helper.

## Pin Map

| Function | Pin |
|---|---:|
| Common TX | `D6` |
| Left RX | `D2` |
| Right RX | `D3` |
| Top RX | `D7` |
| Bottom RX | `D9` |
| Front RX | `D1` |
| Posterior / Back RX | `D8` |
| OLED SDA | `D4` |
| OLED SCL | `D5` |
| NeoPixel data | `D10` |
| Test button | `A0` |
| Built-in LED | `15` |

Physical face naming in the code:

```text
Face 1 -> Bottom
Face 2 -> Front
Face 3 -> Left
Face 4 -> Right
Face 5 -> Back / Posterior
Face 6 -> Top
```

## Communication

The sketch sends `PacketData` through `Serial1` at 19200 baud using `D6` as TX. Each face listens with a `SoftwareSerial` RX-only port.

```cpp
struct PacketData {
  uint8_t state;
  uint8_t neighborCount[2];
};
```

The packet helper wraps payloads with:

```text
START_MARKER 0xAA
payload
checksum complement
END_MARKER 0x55
```

When a packet is received on a face, that face is marked as connected. If no valid packet arrives for 3000 ms, that face is marked as disconnected.

## Interface Behavior

- OLED eyes blink when the node is alone.
- OLED eyes wink when a small number of neighbors are connected.
- OLED eyes glance left or right when three or more faces are connected.
- Small OLED face markers show available connection points and disappear after connection.
- LEDs breathe purple while alone.
- LEDs breathe blue/green when connected.
- Three or more neighbors increase blue intensity and speed up the breathing rhythm.

## Dice-State Logic

`selfState` acts like a dice seed for each node. On startup it gets a random value from 5 to 254. During operation, the node averages the `state` values received from active neighbors and interpolates toward that value.

```text
targetValue = average(active neighbor states)
selfState = lerp(selfState, targetValue, 0.187)
blinkInterval = selfState * 3
```

This gives the network a gradual shared behavior instead of a hard on/off response.

## Related Documentation

- Manual: `MANUALS/node-sprint4-integration.html`
- PCB fabrication package: `BUILD-FILES/pcb-designs/fab-vine-node-sprint3/`
- PCB integration notes: `BUILD-FILES/pcb-designs/fab-vine-node-sprint4/README.md`

