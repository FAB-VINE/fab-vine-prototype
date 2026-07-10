#include "NodeEyes.h"

void NodeEyes::update(const bool(& hasNeighbor)[FACE_COUNT])
{
    display.clearDisplay();

    switch (currentExpression) {
    case EyesBlink:
        eyesBlink();
        break;
    case EyesWink:
        eyesWink();
        break;
    case EyesLookLeft:
        eyesLookLeft();
        break;
    case EyesLookRight:
        eyesLookRight();
        break;
    case EyesNormal:
    default:
        eyesNormal();
        break;
    }

    drawFaceConnectionMarkers(hasNeighbor);

    drawConnectedFaceLetters(hasNeighbor);
    
    display.display();
}

void NodeEyes::setEyesExpression(EyeExpression expression)
{
    currentExpression = expression;
}

void NodeEyes::drawFaceConnectionMarkers(const bool(& hasNeighbor)[FACE_COUNT])
{
    // Physical face numbers:
    // Markers are visible while a face is waiting for a neighbor, and disappear once connected.
    if (!hasNeighbor[FaceBottom]) {
        display.drawFastHLine(60, 60, 8, SSD1306_WHITE);
    }
    if (!hasNeighbor[FaceFront]) {
        display.drawFastHLine(2, 61, 8, SSD1306_WHITE);
        display.drawFastVLine(2, 53, 8, SSD1306_WHITE);
    }
    if (!hasNeighbor[FaceLeft]) {
    display.drawFastHLine(2, 2, 8, SSD1306_WHITE);
    display.drawFastVLine(2, 2, 8, SSD1306_WHITE);
    }
    if (!hasNeighbor[FacePosterior]) {
    display.drawFastHLine(118, 2, 8, SSD1306_WHITE);
    display.drawFastVLine(125, 2, 8, SSD1306_WHITE);
    }
    if (!hasNeighbor[FaceRight]) {
    display.drawFastHLine(118, 61, 8, SSD1306_WHITE);
    display.drawFastVLine(125, 53, 8, SSD1306_WHITE);
    }
    if (!hasNeighbor[FaceTop]) {
    display.drawFastHLine(60, 3, 8, SSD1306_WHITE);
    }
}

void NodeEyes::eye(int x, int y, int w, int h)
{
    display.fillRoundRect(x, y, w, h, 8, SSD1306_WHITE);
}

void NodeEyes::pupils(int x1, int y1, int x2, int y2)
{
    display.fillCircle(x1, y1, 4, SSD1306_BLACK);
    display.fillCircle(x2, y2, 4, SSD1306_BLACK);
}

void NodeEyes::eyesNormal()
{
    eye(18, 18, 32, 24);
    eye(78, 18, 32, 24);
    pupils(34, 30, 94, 30);
}
void NodeEyes::eyesBlink()
{
    eye(18, 28, 32, 5);
    eye(78, 28, 32, 5);
}
void NodeEyes::eyesWink() {
  eye(18, 28, 32, 5);
  eye(78, 18, 32, 24);
  display.fillCircle(94, 30, 4, SSD1306_BLACK);
}

void NodeEyes::eyesLookLeft() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(26, 30, 86, 30);
}

void NodeEyes::eyesLookRight() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(42, 30, 102, 30);
}


void NodeEyes::drawConnectedFaceLetters(const bool(& hasNeighbor)[FACE_COUNT]){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 2);
    display.setCursor(20, 50);
    display.print(hasNeighbor[FaceLeft] ? "L" : ".");
    display.print(hasNeighbor[FaceRight] ? "R" : ".");
    display.print(hasNeighbor[FaceTop] ? "T" : ".");
    display.print(hasNeighbor[FaceBottom] ? "B" : ".");
    display.print(hasNeighbor[FaceFront] ? "F" : ".");
    display.print(hasNeighbor[FacePosterior] ? "P" : ".");
}
