#pragma once

class Cube {
private:
  // vertices
    static float vertices[];

  // faces as triangle triangles
    static int faces[];
    static float DISTANCE_FROM_SCREEN;

  /*
   * faceIndex Should be an array of size 6.
   * vantagePoint should be an array of size 3.
   * */
  static bool isFaceVisible(const int faceIndex[], const float vantagePoint[]);

  static float* calculateCrossProduct(const int* facePointer);
  static float calculateDotProduct(float* faceNormal, const float* vantagePoint,
                                   float* pointOnFace);

public:
  // method to build the vertex array for rendering. It should filter out non-
  //  visible faces.
  static float* generateBufferData(const float vantagePoint[]);

  // method to get the number of visible faces given a vantage point
  static int getNumberOfVisibleFaces(const float vantagePoint[]);
};
