# OLED Expressions Manual

This folder documents the OLED expression system for Fab Vine Node V1.

The OLED display gives each Fab Vine node a simple visible personality. Expressions should communicate state clearly, but still feel organic and alive instead of looking like static interface icons.

## Current Expression Set

The current firmware cycles through these expressions:

- `NORMAL` - neutral open eyes with centered pupils.
- `BLINK` - both eyes briefly closed.
- `WINK` - one eye closed, one eye open.
- `SURPRISED` - taller open eyes with centered pupils.
- `LOOK_LEFT` - both pupils look left.
- `LOOK_RIGHT` - both pupils look right.
- `YES_FACE` - affirmation gesture using mobile eyes.
- `NO_FACE` - negation gesture using mobile eyes.

## Affirmation Behavior

`YES_FACE` should feel like a soft nod.

Current behavior:

- Both eyes move vertically.
- Pupils stay centered inside the moving eyes.
- The expression updates every `220 ms`.
- The full affirmation lasts around `2500 ms`.
- After affirmation, the firmware advances to `NO_FACE`.

Design intention:

- Use movement instead of checkmark symbols.
- Keep the face natural and character-like.
- Make the gesture readable even on a small `128x64` OLED.

## Negation Behavior

`NO_FACE` should feel like a soft head shake.

Current behavior:

- Both eyes move horizontally from side to side.
- Pupils stay centered inside each moving eye.
- The expression updates every `220 ms`.
- The full negation lasts around `2500 ms`.
- After negation, the firmware returns to `NORMAL`.

Design intention:

- Move the eyes as a unit, not only the pupils.
- Avoid static `X` symbols unless a stronger error state is needed.
- Make the expression more natural and less like signage.

## Timing And Loop Structure

The loop has special handlers for affirmation and negation:

- `updateYesFace()` controls `YES_FACE`.
- `updateNoFace()` controls `NO_FACE`.
- `nextFace()` advances the normal expression sequence.
- `returnToNormal()` resets the face after the negation cycle.

Important behavior:

- `YES_FACE` must call `nextFace()` when it finishes, so the cycle can continue into `NO_FACE`.
- `NO_FACE` can call `returnToNormal()` when it finishes, because it is the last expression in the current sequence.

This is important because returning to `NORMAL` directly from `YES_FACE` skips `NO_FACE`.

## Preview Tool

Before uploading to the board, use the browser preview:

```text
NODE-LOGIC/fab-vine-node/V1/tools/oled-expression-preview.html
```

Recommended local URL:

```text
http://127.0.0.1:4174/NODE-LOGIC/fab-vine-node/V1/tools/oled-expression-preview.html
```

The preview should match the Arduino drawing logic as closely as possible. When changing a face in the firmware, update the preview too.

## Arduino Upload Notes

Current tested board:

- Seeed XIAO ESP32-C6 / ESP32-C6 family board
- Port used during testing: `COM3`
- FQBN used for upload: `esp32:esp32:XIAO_ESP32C6`

Current OLED assumptions:

- Display: `128x64`
- Driver: `SSD1306`
- I2C address: `0x3C`
- SDA: `D4`
- SCL: `D5`

Required Arduino libraries:

- `Adafruit GFX Library`
- `Adafruit SSD1306`

## Editing Guidelines

When adding or changing an expression:

- Name the expression in the `Face` enum.
- Add the drawing logic in a dedicated function.
- Keep movement readable at small OLED scale.
- Avoid relying on very subtle pupil changes.
- Update the preview HTML.
- Compile before upload.
- Test on the physical OLED.
- Document the behavior here.

## Espanol

Este manual documenta las expresiones OLED del nodo Fab Vine V1.

La afirmacion usa ojos moviles en vertical, como un gesto de si. La negacion usa ojos moviles en horizontal, como un gesto de no. Las pupilas se mantienen centradas dentro de cada ojo para que el movimiento se vea mas natural.

Cuando se cambie una expresion, hay que actualizar tambien el simulador local antes de subir el firmware al Arduino.
