#version 330

varying vec2 texCoord0;
varying vec3 position0;
uniform sampler2D diffuse;
void main() {
  
  gl_FragColor = glm::vec4(1.0,0.0,0.0,1.0);
}
