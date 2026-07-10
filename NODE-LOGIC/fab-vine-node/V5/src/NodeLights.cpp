#include "NodeLights.h"

void NodeLights::update(const unsigned long currentMillis) {

  unsigned long deltaTime = (currentMillis - lastUpdate);
  // do nothing if the refresh delay has not passed yet
  if (deltaTime < refreshDelayMS) return;
  lastUpdate = currentMillis;
  
  currentColor = Color::LinearBlend(currentColor, targetColor, 0.1f);

  if (isPointing) {
    animateToface(pointColor, deltaTime);
    if(pointDuration > 0){
      pointDuration -= deltaTime;
      if(pointDuration <= 0){
        isPointing = false;
      }
    }
  } else {
    breathFromCenter(currentColor, deltaTime);
  }

  pixels.show();
}

void NodeLights::pointToFace(Face face, Color color) {
  isPointing = true;
  pointColor = color;
  if (targetFace != face) {
    targetFace = face;
    directionAnimationTime = 0;
  }
}
void NodeLights::pointToFace(Face targetFace, Color color, const unsigned long pointDurationMs)
{
  pointDuration = pointDurationMs;
  pointToFace(targetFace, color);
}
void NodeLights::animateToface(Color &color, const unsigned long deltaTime) {
  // handle timing
  directionAnimationTime += deltaTime;
  if (directionAnimationTime >= directionDurationMs) {
    directionAnimationTime = 0;
  }
  float phase = directionAnimationTime / directionDurationMs;

  pixels.clear();

  float travel = phase * 1.35f - 0.17f;
  int targetGroup = pointGroups[targetFace][0];
  int reverseTargetGroup = pointGroups[targetFace][1];

  bool verticalDirection = targetFace == FaceTop || targetFace == FaceBottom;

  for (uint8_t row = 0; row < ledsPerGroup; row++) {
    float position = (float)row / (float)(ledsPerGroup - 1);

    if (targetFace == FaceBottom) {
      position = 1.0f - position; // invert position
    }
    float distance = (position - travel) / 0.18f;
    float glow = exp(-(distance * distance));

    uint8_t level = 2 + (uint8_t)(190.0f * glow);
    uint32_t encoded_color = pixels.Color((color.r * level) / 255,
                                  (color.g * level) / 255,
                                  (color.b * level) / 255);

    uint8_t pixelIndex = (targetGroup * ledsPerGroup) + (row);
    pixels.setPixelColor(pixelIndex, encoded_color);
    if (!verticalDirection) {
      // create helix pattern
      pixelIndex =
          (reverseTargetGroup * ledsPerGroup) + (ledsPerGroup - 1 - row);
    } else {
      pixelIndex = (reverseTargetGroup * ledsPerGroup) + row;
    }
    pixels.setPixelColor(pixelIndex, encoded_color);
  }
}
void NodeLights::breathFromCenter(Color &color, const unsigned long deltaTime) {
  breathAnimationTime += deltaTime;
  if (breathAnimationTime >= breathDurationMs) {
    breathAnimationTime = 0;
  }
  float phase = breathAnimationTime / breathDurationMs;

  // breath
  float breath = 0.0f;

  // Respiración asimétrica: inhala, sostiene un instante, exhala y descansa.
  if (phase < 0.40f) {
    float inhale = phase / 0.40f;
    breath = inhale * inhale * (3.0f - 2.0f * inhale);
  } else if (phase < 0.50f) {
    breath = 1.0f;
  } else if (phase < 0.90f) {
    float exhale = (phase - 0.50f) / 0.40f;
    exhale = exhale * exhale * (3.0f - 2.0f * exhale);
    breath = 1.0f - exhale;
  }

  for (uint8_t column = 0; column < groupCount; column++) {
    for (uint8_t row = 0; row < ledsPerGroup; row++) {
      // Con diez LEDs, el centro físico está entre las posiciones 4 y 5.
      // Ambas se encienden juntas y la respiración se abre hacia los extremos.
      float distanceFromCenter = abs((float)row - 4.5f);
      float outwardPosition = (distanceFromCenter - 0.5f) / 4.0f;

      // El cuerpo luminoso, el frente y una segunda estela se mezclan entre sí.
      float fill =
          constrain((breath - outwardPosition + 0.22f) / 0.42f, 0.0f, 1.0f);
      fill = fill * fill * (3.0f - 2.0f * fill);
      float distanceToFront = (outwardPosition - breath) / 0.23f;
      float trailGlow = exp(-(distanceToFront * distanceToFront));
      float amount =
          0.022f + (0.56f * fill) + (0.31f * trailGlow) + (0.16f * trailGlow);
      amount *= 0.74f + (0.26f * breath);
      amount = constrain(amount, 0.0f, 1.0f);

      // Cada bloque de diez está cableado del LED inferior al superior.
      uint8_t pixelIndex = (column * ledsPerGroup) + row;
      uint8_t level = 4 + (uint8_t)(160.0f * amount);
      pixels.setPixelColor(pixelIndex,
                           pixels.Color((currentColor.r * level) / 255,
                                        (currentColor.g * level) / 255,
                                        (currentColor.b * level) / 255));
    }
  }
}

void NodeLights::setColor(uint8_t r, uint8_t g, uint8_t b) {
  targetColor.r = r;
  targetColor.g = g;
  targetColor.b = b;
}
