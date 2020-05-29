#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <iostream>

#include <glm/gtx/transform.hpp>
#include "mesh.h"
class Camera {
public:
  glm::mat4 m_perspective;
  glm::vec3 m_position;
  glm::vec3 m_forward;
  glm::vec3 m_up;
  glm::vec3 m_mainPos;
  float shakeAmount = 0.0f;
  Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) {
    m_position = pos;
    m_forward = glm::vec3(0,0,1);
    m_up = glm::vec3(0,1,0);
    m_perspective = glm::perspective(fov, aspect,zNear,zFar);
  }
  Camera() {}
  void InitCam(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) {
    m_position = pos;
    m_forward = glm::vec3(0,0,1);
    m_up = glm::vec3(0,1,0);
    m_perspective = glm::perspective(fov, aspect,zNear,zFar);
  }
  inline glm::mat4 GetViewProjection() const
  {
    return m_perspective * glm::lookAt(m_position,m_position+m_forward, m_up);
  }
  
  void MoveForward(float amt)
	{
		m_position += m_forward * amt;
	}
  void Shake(float amount) {
    shakeAmount = amount;
  }
  void UpdateShake() {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    m_position = m_position+((r-0.5f)*shakeAmount);
    if (shakeAmount > 0.0f) {
      shakeAmount-=0.1f;
    }else {
      shakeAmount = 0.0f;
    }
  }

	void MoveRight(float amt)
	{
		m_position += glm::cross(m_up, m_forward) * amt;
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(m_up, m_forward));

		m_forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(m_forward, 0.0)));
		m_up = glm::normalize(glm::cross(m_forward, right));
	}

	void RotateY(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		m_forward = glm::vec3(glm::normalize(rotation * glm::vec4(m_forward, 0.0)));
		m_up = glm::vec3(glm::normalize(rotation * glm::vec4(m_up, 0.0)));
    // std::cout << "Forward: " << m_forward.y << std::endl;
	}
protected:
private:

};
#endif
