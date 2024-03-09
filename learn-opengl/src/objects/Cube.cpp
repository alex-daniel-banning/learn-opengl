#pragma once

#include <vector>
#include <objects/Cube.h>

float Cube::vertices[] = {
    -0.5f, -0.5f,  2.5f,
    -0.5f,  0.5f,  2.5f,
     0.5f,  0.5f,  2.5f,
     0.5f, -0.5f,  2.5f,
    -0.5f, -0.5f,  3.5f,
    -0.5f,  0.5f,  3.5f,
     0.5f,  0.5f,  3.5f,
     0.5f, -0.5f,  3.5f
};
int Cube::faces[] = {
    0, 1, 2,  2, 3, 0,
    7, 6, 5,  5, 1, 7,
    4, 5, 1,  1, 0, 4,
    3, 2, 6,  6, 0, 3,
    1, 5, 6,  6, 2, 1,
    3, 7, 4,  4, 0, 3
};
float Cube::DISTANCE_FROM_SCREEN = 1.0f;

bool Cube::isFaceVisible(const int faceIndex[],
                                const float vantagePoint[]) {
  float* crossProduct = calculateCrossProduct(faceIndex);
  float dotProduct = calculateDotProduct(crossProduct, vantagePoint, vertices + (faceIndex[0] * 3));
  delete[] crossProduct;
  return dotProduct < 0;
}

float* Cube::calculateCrossProduct(const int* facePointer) {
  std::vector<float> v1 = {
    vertices[facePointer[1]] - vertices[facePointer[0]],
    vertices[facePointer[1 + 1]] - vertices[facePointer[0 + 1]],
    vertices[facePointer[1 + 2]] - vertices[facePointer[0 + 2]]
  };
  std::vector<float> v2 = {
    vertices[facePointer[2]] - vertices[facePointer[1]],
    vertices[facePointer[2 + 1]] - vertices[facePointer[1 + 1]],
    vertices[facePointer[2 + 2]] - vertices[facePointer[1 + 2]]
  };

  float* result = new float[3];
  result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
  result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
  result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
  return result;
}

float Cube::calculateDotProduct(float* faceNormal, const float* vantagePoint,
    float* pointOnFace) {
  std::vector<float> eyelineVector = {
    vantagePoint[0] - pointOnFace[0],
    vantagePoint[1] - pointOnFace[1],
    vantagePoint[2] - pointOnFace[2]
  };

  return (faceNormal[0] * eyelineVector[0]) +
         (faceNormal[1] * eyelineVector[1]) +
         (faceNormal[2] * eyelineVector[2]);
}

float* Cube::generateBufferData(const float vantagePoint[]) {
  // for each visible face, generate the 2.5D coordinates
  std::vector<float> bufferData;
  for (int faceNumber = 0; faceNumber < 6; faceNumber++) {
    if (isFaceVisible(faces + (faceNumber * 6), vantagePoint)) {
      // calculate the 2.5D coordinates for the triangle vertices of the face
      for (int triangleNumber = 0; triangleNumber <= 3; triangleNumber+=3) {
        float pX = vertices[faces[faceNumber * 6] + (triangleNumber)];
        float pY = vertices[faces[faceNumber * 6] + (triangleNumber + 1)];
        float pZ = vertices[faces[faceNumber * 6] + (triangleNumber + 2)];
        float eX = vantagePoint[0];
        float eY = vantagePoint[1];
        float eZ = vantagePoint[2];
        float xPrime = ((pX - eX) * DISTANCE_FROM_SCREEN) / (pZ- eZ);
        float yPrime = ((pY - eY) * DISTANCE_FROM_SCREEN) / (pZ- eZ);
        float zPrime = 0.0f;
        bufferData.push_back(xPrime);
        bufferData.push_back(yPrime);
        bufferData.push_back(zPrime);
      }
    }
  }
  //convert bufferData vector to array
  float* result = new float[bufferData.size()];
  const float* data_ptr = bufferData.data();
  std::copy(data_ptr, data_ptr + bufferData.size(), result);
  return result;
};

int Cube::getNumberOfVisibleFaces(const float vantagePoint[]) {
    int numberOfVisibleFaces = 0;
    for (int i = 0; i < 6; i++) {
        if (isFaceVisible(faces + (i * 6), vantagePoint)) {
            numberOfVisibleFaces++;
        }
    }
    return numberOfVisibleFaces;
};

