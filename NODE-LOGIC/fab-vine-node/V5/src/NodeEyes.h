#ifndef NODE_EYES_H
#define NODE_EYES_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Faces.h"

enum EyeExpression : uint8_t { EyesNormal,
                               EyesBlink,
                               EyesWink,
                               EyesLookLeft,
                               EyesLookRight };

class NodeEyes
{
private:
    Adafruit_SSD1306& display;
    EyeExpression currentExpression;
    /* data */
public:
    NodeEyes(Adafruit_SSD1306& oledDisplay)
        :display{oledDisplay}
        {}
    void update(const bool (& hasNeighbor)[FACE_COUNT]);
    void setEyesExpression(EyeExpression expression);
    const EyeExpression getCurrentExpression(){return currentExpression;};
private: 
    void drawFaceConnectionMarkers(const bool (& hasNeighbor)[FACE_COUNT]);
    void eye(int x, int y, int w, int h);
    void pupils(int x1, int y1, int x2, int y2);
    void eyesNormal();
    void eyesBlink();
    void eyesWink();
    void eyesLookLeft();
    void eyesLookRight();
    void drawConnectedFaceLetters(const bool(& hasNeighbor)[FACE_COUNT]);
};

#endif // NODE_EYES_H