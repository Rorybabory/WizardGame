#version 120

varying vec2 texCoord0;
varying vec3 normal0;
varying int x;
varying int y;
uniform sampler2D diffuse;

void main() {
  vec4 texColor = texture2D(diffuse,texCoord0);
  gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
