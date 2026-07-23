#ifndef FACES_H
#define FACES_H

#define FACE_COUNT 6

enum Face : uint8_t {
  FaceLeft,
  FaceRight,
  FaceTop,
  FaceBottom,
  FaceFront,
  FacePosterior
};

const Face OPP_Face[FACE_COUNT]  {
  FaceRight,
  FaceLeft,
  FaceBottom,
  FaceTop,
  FacePosterior,
  FaceFront
};

const int8_t face_dir_vector[FACE_COUNT][3] = {
  {1,0,0}, 
  {-1,0,0}, 
  {0,1,0}, 
  {0,-1,0}, 
  {0,0,1}, 
  {0,0,-1}
};

// build priority: trunk first, then lateral, then down
const uint8_t BUILD_PRIORITY[FACE_COUNT] = { 
  FaceTop,
  FaceRight,
  FaceLeft,
  FacePosterior,
  FaceFront,
  FaceBottom
};
#endif // FACES_H