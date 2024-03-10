#pragma once

class Cube {
private:
  // vertices
    static std::vector<float> vertices;

  // faces as triangle triangles
    static std::vector<int> faces;
    static float DISTANCE_FROM_SCREEN;

  /*
   * faceIndex Should be an array of size 6.
   * vantagePoint should be an array of size 3.
   * */
  static bool isFaceVisible(const std::vector<int> face, const std::vector<float> vantagePoint);

  static std::vector<float> calculateCrossProduct(const std::vector<int> facePointer);
  
  static float calculateDotProduct(const std::vector<float> faceNormal, const std::vector<float> vantagePoint,
                                   const std::vector<float> pointOnFace);

public:
  // method to build the vertex array for rendering. It should filter out non-
  //  visible faces.
  static float* generateBufferData(const std::vector<float> vantagePoint);

  // method to get the number of visible faces given a vantage point
  static int getNumberOfVisibleFaces(const std::vector<float> vantagePoint);
};
