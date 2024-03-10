#pragma once

#include <vector>
#include <objects/Cube.h>

std::vector<float> Cube::vertices = {
    -0.5f, -0.5f,  2.5f,
    -0.5f,  0.5f,  2.5f,
     0.5f,  0.5f,  2.5f,
     0.5f, -0.5f,  2.5f,
    -0.5f, -0.5f,  3.5f,
    -0.5f,  0.5f,  3.5f,
     0.5f,  0.5f,  3.5f,
     0.5f, -0.5f,  3.5f
};
std::vector<int> Cube::faces = {
    0, 1, 2,  2, 3, 0,
    7, 6, 5,  5, 4, 7,
    4, 5, 1,  1, 0, 4,
    3, 2, 6,  6, 7, 3,
    1, 5, 6,  6, 2, 1,
    3, 7, 4,  4, 0, 3
};
float Cube::DISTANCE_FROM_SCREEN = 1.0f;

bool Cube::isFaceVisible(const std::vector<int> face,
                                const std::vector<float> vantagePoint) {
  const std::vector<float> crossProduct = calculateCrossProduct(face);
  const std::vector<float> pointOnFace = {
      vertices[(3 * face[0])],
      vertices[(3 * face[0]) + 1],
      vertices[(3 * face[0]) + 2]
  };
  float dotProduct = calculateDotProduct(crossProduct, vantagePoint, pointOnFace);
  return dotProduct < 0;
}

std::vector<float> Cube::calculateCrossProduct(const std::vector<int> face) {
  std::vector<float> v1 = {
      vertices[3 * face[1]] - vertices[3 * face[0]],
      vertices[(3 * face[1]) + 1] - vertices[(3 * face[0]) + 1],
      vertices[(3 * face[1]) + 2] - vertices[(3 * face[0]) + 2],
  };
  std::vector<float> v2 = {
      vertices[3 * face[2]] - vertices[3 * face[1]],
      vertices[(3 * face[2]) + 1] - vertices[(3 * face[1]) + 1],
      vertices[(3 * face[2]) + 2] - vertices[(3 * face[1]) + 2],
  };

  std::vector<float> result = {
    (v1[1] * v2[2]) - (v1[2] * v2[1]),
    (v1[2] * v2[0]) - (v1[0] * v2[2]),
    (v1[0] * v2[1]) - (v1[1] * v2[0])
  };
  return result;
}

float Cube::calculateDotProduct(const std::vector<float> faceNormal, const std::vector<float> vantagePoint,
    const std::vector<float> pointOnFace) {
    std::vector<float> eyelineVector = {
      pointOnFace[0] - vantagePoint[0],
      pointOnFace[1] - vantagePoint[1],
      pointOnFace[2] - vantagePoint[2]
    };

  return (faceNormal[0] * eyelineVector[0]) +
         (faceNormal[1] * eyelineVector[1]) +
         (faceNormal[2] * eyelineVector[2]);
}

float* Cube::generateBufferData(const std::vector<float> vantagePoint) {
  // for each visible face, generate the 2.5D coordinates
  std::vector<float> bufferData;
  for (int faceNumber = 0; faceNumber < 36; faceNumber+=6) {
    std::vector<int> faceVector = std::vector<int>(faces.begin() + faceNumber, faces.begin() + faceNumber + 6);
    if (isFaceVisible(faceVector, vantagePoint)) {
      // calculate the 2.5D coordinates for the triangle vertices of the face
      for (int vertexIndex : faceVector) {
        float pX = vertices[3 * vertexIndex];
        float pY = vertices[(3 * vertexIndex) + 1];
        float pZ = vertices[(3 * vertexIndex) + 2];
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

int Cube::getNumberOfVisibleFaces(const std::vector<float> vantagePoint) {
    int numberOfVisibleFaces = 0;
    for (int faceNumber = 0; faceNumber < 36; faceNumber+=6) {
        const std::vector<int> faceVector = std::vector<int>(faces.begin() + faceNumber, faces.begin() + faceNumber + 6);
        if (isFaceVisible(faceVector, vantagePoint)) {
            numberOfVisibleFaces++;
        }
    }
    return numberOfVisibleFaces;
};

