#version 120

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse;

void main() {
  vec4 texColor = texture2D(diffuse,texCoord0);
  if (texColor.a < 0.1) {
    discard;
  }
  gl_FragColor = texColor;

}
