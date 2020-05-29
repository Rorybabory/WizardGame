#version 120

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform sampler2D sampler;
uniform vec4 color;
uniform int inverted;
void main() {
    gl_FragColor = color;
}
