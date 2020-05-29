#include "animatedMesh.h"
#include <thread>
animatedMesh::animatedMesh(std::string fileName) {
  //std::cout << fileName << '\n';
  this->fileName = fileName;
  std::cout << "fileName:" << fileName << "\n";
  Assimp::Importer Importer;
  const aiScene* pScene = Importer.ReadFile(fileName,
                aiProcess_LimitBoneWeights |
                aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType |
                aiProcess_GenUVCoords |
                aiProcess_TransformUVCoords |
                aiProcess_FlipUVs);
  if (!pScene) {
      std::cout << "null scene" << std::endl;
  }
  AssimpConverter::Convert(pScene,sModel);

  const SA::sAnimatedMesh& model1 = sModel.m_Meshes[0];
  
  for (int i = 0; i < model1.NumVertices; i++) {
    verts.push_back(Vertex(model1.pVertices[i],model1.pTexCoords[i],glm::vec3(0,0,0)));
  }
  for (int i2 = 0; i2 < model1.NumIndices; i2++) {
    indices.push_back(model1.pIndices[i2]);
  }

  numVertices = model1.NumVertices;
  glGenVertexArrays(1, &m_vertexArrayObject);
  glBindVertexArray(m_vertexArrayObject);
  glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
  InitMesh();
}
int sA(glm::vec3* arr) {
  return sizeof(arr);
}
animatedMesh::~animatedMesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}
void animatedMesh::calculateMesh() {
  for (int i = 0; i <= Duration; i++) {
      IndexedModel model2;
      sModel.Update((float)(16.0f)/1000);
      model1 = sModel.m_Meshes[0];
      for (int i = 0; i < indices.size(); i++) {
        int f = indices[i];
        model2.positions.push_back(model1.pTransformedVertices[f]);
        model2.normals.push_back(model1.pTransformedNormals[f]);
        model2.texCoords.push_back(model1.pTexCoords[f]);
        // std::cout << "texCoords: " << model1.pTexCoords[f].x << " " << model1.pTexCoords[f].y << std::endl;
      }
      for(unsigned int i = 0; i < indices.size(); i++) {
        model2.indices.push_back(i);
      }
      models.push_back(model2);
      std::cout << "INDICES SIZE: " << indices.size() << std::endl;
  }
}
void animatedMesh::Draw()
{
  if (MeshStored) {
    glBindVertexArray(m_vertexArrayObject);
    glDrawArrays(GL_TRIANGLES,0,numIndices);
    glBindVertexArray(0);
  }
}
void animatedMesh::CalculateSkeletalModel() {
  models.reserve(Duration + 1);
  for (int i = 0; i <= Duration; i++) {
      IndexedModel model2;
      sModel.Update((float)(16.0f)/1000);
      model1 = sModel.m_Meshes[0];
      model2.positions.reserve(indices.size());
      model2.normals.reserve(indices.size());
      model2.texCoords.reserve(indices.size());
      model2.indices.reserve(indices.size());
      for (int i = 0; i < indices.size(); i++) {
        int f = indices[i];
        model2.positions.push_back(model1.pTransformedVertices[f]);
        model2.normals.push_back(model1.pTransformedNormals[f]);
        model2.texCoords.push_back(model1.pTexCoords[f]);
      }
      for(unsigned int i = 0; i < indices.size(); i++) {
        model2.indices.push_back(i);
      }
      models.push_back(model2);
  }
}
void animatedMesh::UpdateMesh(const IndexedModel& model) {
  if (MeshStored) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_DYNAMIC_DRAW);
  }
}
glm::vec3 animatedMesh::getMin() {return models[renderFrame].min; }
glm::vec3 animatedMesh::getMax() {return models[renderFrame].max; }
void animatedMesh::restartMeshMinMax() {
  // std::cout << "restarted Min Max" << "\n";
  // for (IndexedModel Selected : models) {
  //   glm::vec3 tempMin = Selected.min;
  //   glm::vec3 tempMax = Selected.max;
  //   Selected.min = glm::vec3(1000.0f,1000.0f,1000.0f);
  //   Selected.max = glm::vec3(-1000.0f,-1000.0f,-1000.0f);
  //   for (glm::vec3 positions : Selected.positions) {
  //     if (
  //        positions.x < 500.0f && positions.y < 500.0f && positions.z < 500.0f &&
  //        positions.x > -500.0f && positions.y > -500.0f && positions.z > -500.0f &&
  //         positions.x == positions.x) {
  //       if (positions.x > Selected.max.x) {Selected.max.x = positions.x;}
  //       if (positions.y > Selected.max.y) {Selected.max.y = positions.y;}
  //       if (positions.z > Selected.max.z) {Selected.max.z = positions.z;}

  //       if (positions.x < Selected.min.x) {Selected.min.x = positions.x;}
  //       if (positions.y < Selected.min.y) {Selected.min.y = positions.y;}
  //       if (positions.z < Selected.min.z) {Selected.min.z = positions.z;}
  //     }else {
  //       std::cout << "AAAAAAAAAAAAAAAAA WRONG POSITION" << "\n";
  //     }
  //   }



  // }
}
bool animatedMesh::Update(int renderFrame) {
  this->renderFrame = renderFrame;
//  std::cout << "Duration: " << sModel.m_Animation.Duration << "\n";
  if (MeshStored) {
      if (models.size() == 0) {
          models.reserve((int)(Duration+1.0f));
      }
    while (models.size()<=renderFrame) {
      IndexedModel model2;
      sModel.Update((float)(16.0f)/1000);
      model1 = sModel.m_Meshes[0];
      model2.positions.reserve(indices.size());
      model2.normals.reserve(indices.size());
      model2.texCoords.reserve(indices.size());
      model2.indices.reserve(indices.size());

      for (int i = 0; i < indices.size(); i++) {
        int f = indices[i];
        
        model2.positions.push_back(model1.pTransformedVertices[f]);
        model2.normals.push_back(model1.pTransformedNormals[f]);
        model2.texCoords.push_back(model1.pTexCoords[f]);
        // std::cout << "texCoords: " << model1.pTexCoords[f].x << " " << model1.pTexCoords[f].y << std::endl;
      }
      for(unsigned int i = 0; i < indices.size(); i++) {
        model2.indices.push_back(i);
      }
      // model2.CalcNormals();
      models.push_back(model2);
      // std::cout << "Updating Mesh Real Time..." << '\n';
    }
          // std::cout << "INDICES SIZE: " << indices.size() << std::endl;

    if (renderFrame>Duration) {
      frame = 0;
      UpdateMesh(models[renderFrame]);
      return true;
    }
    if (!models.empty()) {

      UpdateMesh(models[renderFrame]);
    }
    return false;
  }
}
// void animatedMesh::InitMeshThread(const IndexedModel& model) {
//   modelSave = model;
//   numIndices = indices.size();
// glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
// glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_DYNAMIC_DRAW);
//   glEnableVertexAttribArray(0);
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
// glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
// glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_DYNAMIC_DRAW);
// glEnableVertexAttribArray(2);
// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
// glBindVertexArray(0);
// // std::cout << "Animation Time: " << sModel.m_Animation.Duration*60.0f << '\n';
// Duration = sModel.m_Animation.Duration*(sModel.m_Animation.Duration*60.0f);
// std::cout << "Durration = " << Duration << " for: " << fileName << '\n';
// for (float i = 0; i<=Duration; i++) {
//   IndexedModel model2;
//   sModel.Update((float)(16.0f)/1000);
//   model1 = sModel.m_Meshes[0];
//   for (int i = 0; i < indices.size(); i++) {
//     int f = indices[i];
//     model2.positions.push_back(model1.pTransformedVertices[f]);
//     model2.normals.push_back(model1.pTransformedNormals[f]);
//   }
//   for(unsigned int i = 0; i < indices.size(); i++) {
//     model2.indices.push_back(i);
//   }
//   models.push_back(model2);
//   // std::cout << "Updated " << i << "..." << '\n';
//
// }
// MeshStored = true;
// }
void animatedMesh::InitMeshModel(const IndexedModel& model)
{
  // std::thread init (&animatedMesh::InitMeshThread,this,model);
  // init.join();
  // std::cout << "INIT CALLED..." << '\n';
  modelSave = model;
  numIndices = indices.size();

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


  glBindVertexArray(0);
  // std::cout << "Animation Time: " << sModel.m_Animation.Duration*60.0f << '\n';
  Duration = sModel.m_Animation.Duration*57.5998;
  // std::cout << "Durration = " << Duration << " for: " << fileName << '\n';
  MeshStored = true;
}

void animatedMesh::InitMesh()
{
  IndexedModel model;
  for (int i = 0; i < indices.size(); i++) {
    int f = indices[i];
    model.positions.push_back(*verts[f].GetPos());
  }
for(unsigned int i = 0; i < numVertices; i++)
{
  model.positions.push_back(*verts[i].GetPos());
  model.texCoords.push_back(*verts[i].GetTexCoord());
  model.normals.push_back(*verts[i].GetNormal());
}
for(unsigned int i = 0; i < indices.size(); i++) {
  model.indices.push_back(i);
}
InitMeshModel(model);
}
