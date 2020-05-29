#version 120
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 normal0;
varying vec2 texCoord0;
uniform mat4 transform;
void main() {
  gl_Position = vec4(position, 1.0);
  texCoord0 = texCoord;
  normal0 = (vec4(normal, 0.0)).xyz;
}
