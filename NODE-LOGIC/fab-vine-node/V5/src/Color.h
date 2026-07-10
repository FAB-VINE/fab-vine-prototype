#ifndef COLOR_H
#define COLOR_H

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b){};

  static Color LinearBlend(Color left, Color right, float progress) {
    return Color(left.r + ((right.r - left.r) * progress),
                 left.g + ((right.g - left.g) * progress),
                 left.b + ((right.b - left.b) * progress));
  }
};

#endif // COLOR_H