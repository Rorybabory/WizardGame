#ifndef AMESH_H
#define AMESH_H
#include <string>
#include <iostream>
#include "SkeletalAnimation/Types.h"
#include "SkeletalAnimation/SkeletalModelSerializer.h"
#include "SkeletalAnimation/SkeletalModel.h"
#include "AssimpConverter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "mesh.h"
#include "transform.h"

class animatedMesh {
public:
  animatedMesh(std::string fileName);
  SA::SkeletalModel sModel;
  SA::sAnimatedMesh model1;
  void InitMesh();
  void InitMeshModel(const IndexedModel& model);
  void Draw();
  bool Update(int renderFrame);
  void CalculateSkeletalModel();
  void UpdateMesh(const IndexedModel& model);
  void InitMeshThread(const IndexedModel& model);
  glm::vec3 getMin();
  glm::vec3 getMax();
  virtual ~animatedMesh();
  void calculateMesh();
  void restartMeshMinMax();
  void runDebug() {
    std::cout << "fileName is: " << fileName << "\n";
  }
  unsigned int numI = 0;
  IndexedModel modelSave;
  IndexedModel model;
  std::vector<IndexedModel> models;
  float count = 0;
  glm::vec4 color;
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 minNoPos;
  glm::vec3 maxNoPos;
  float frame = 0;
  float Duration;
  std::string fileName;
  bool MeshStored;
  double frameDouble = 1;
  int renderFrame = 0;
  Transform * t;
protected:
private:
  enum{
    POSITION_VB,
    TEXCOORD_VB,
    NORMAL_VB,
    INDEX_VB,
    NUM_BUFFERS
  };
  GLuint vertexBuffer;
  GLuint m_vertexArrayObject;
  GLuint m_vertexArrayBuffers[NUM_BUFFERS];
  std::vector<Vertex> verts;
  std::vector<unsigned int> indices;

  int numVertices = 0;
  int numIndices = 0;
};

#endif
