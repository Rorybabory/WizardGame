#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "obj_loader.h"
#include "SkeletalAnimation/Types.h"
#include "SkeletalAnimation/SkeletalModelSerializer.h"
#include "SkeletalAnimation/SkeletalModel.h"
#include "AssimpConverter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Vertex {
public:
  Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0,0,0)) {
    this->pos = pos;
    this->texCoord=texCoord;
    this->normal=normal;
  }
  Vertex() {

  }
  inline glm::vec3* GetPos() {return &pos;}
  inline glm::vec2* GetTexCoord() {return &texCoord;}
  inline glm::vec3* GetNormal() {return &normal;}
protected:
private:
  glm::vec3 pos;
  glm::vec2 texCoord;
  glm::vec3 normal;
};

class Mesh
{
    public:
        Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
        Mesh(const std::string& fileName,bool hasUVs);
        Mesh(std::vector<glm::vec3> v, std::vector<glm::vec3> n, std::vector<unsigned int> t);
        Mesh() {}
        void Draw();
        void UpdateMinMax();
        virtual ~Mesh();
        void InitMesh(const IndexedModel& model);
        glm::vec3 min;
        glm::vec3 max;
        IndexedModel modelSave;
    protected:
    private:
        // Mesh(const Mesh& other);
        // void operator=(const Mesh& other);

        enum{
          POSITION_VB,
          TEXCOORD_VB,
          NORMAL_VB,
          INDEX_VB,
          NUM_BUFFERS
        };
        Vertex vertices;
        GLuint m_vertexArrayObject;
        GLuint m_vertexArrayBuffers[NUM_BUFFERS];
        unsigned int m_numIndices;

};

#endif // MESH_H
