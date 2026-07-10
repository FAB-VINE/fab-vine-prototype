#ifndef NODE_LIGHTS_H
#define NODE_LIGHTS_H

#include <Adafruit_NeoPixel.h>

#include "Color.h"
#include "Faces.h"
// #define IS_MIRROR false
class NodeLights {

private:
  Color currentColor;
  Color targetColor;
  Color pointColor;
  Adafruit_NeoPixel &pixels;
  Face targetFace = FaceBottom;
  bool isPointing = false;

  // animation timing
  const unsigned long refreshDelayMS = 30;
  unsigned long lastUpdate = 0;
  float breathAnimationTime = 0;
  float directionAnimationTime = 0;
  float breathDurationMs = 6000;     // 6s to breath
  float directionDurationMs = 2800;  // 2.5s direction animation

  float pointDuration = -1.0f;

  const uint8_t ledsPerGroup = 10;
  const uint8_t groupCount = 4;
  
  const uint8_t pointGroups[6][2] = {
    #if IS_MIRROR
    { 1, 0 },  // left
    { 3, 2 }, // right
    { 0, 2 },  // top
    { 1, 3 },  // bottom
    { 2, 1 },  // front
    { 0, 3 },  // posterior
    #else
    { 2, 1 },  // left
    { 0, 3 },  // right
    { 0, 2 },  // top
    { 1, 3 },  // bottom
    { 1, 0 },  // front
    { 3, 2 },  // posterior
    #endif
  };

public:
  NodeLights(Adafruit_NeoPixel &neoPixels)
    : pixels(neoPixels), 
      currentColor(0, 0, 0), 
      targetColor(0, 0, 0), 
      pointColor(0,0,0)
    {}
  void update(unsigned long currentMillis);
  void setColor(uint8_t r, uint8_t g, uint8_t b);
  void pointToFace(Face face, Color color);
  void pointToFace(Face targetFace, Color color, const unsigned long pointDuration);
  void breath() {
    isPointing = false;
  }
  bool IsPointing() const {return isPointing;}

private:
  void breathFromCenter(Color &color, const unsigned long currentMillis);
  void animateToface(Color &color, const unsigned long currentMillis);
};


#endif  // NODE_LIGHTS_H