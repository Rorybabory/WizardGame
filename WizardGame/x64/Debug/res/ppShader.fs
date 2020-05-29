#version 330 core

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform sampler2D sampler;
uniform vec4 color;
uniform int inverted;
const float offset = 1.0 / 300.0;
void main() {
    bool isEdge = false;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );
    float kernelEdge[9] = float[](
        1, 1, 1,
        1,  -8.0, 1,
        1, 1, 1
    );
    float kernelBlur[9] = float[](
        1/16, 2/16, 1/16,
        2/16, 4/16, 2/16,
        1/16, 2/16, 1/16
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(sampler, texCoord0.st + offsets[i]));
    }
    vec3 colEdge = vec3(0.0);
    for(int i = 0; i < 9; i++)
        colEdge += sampleTex[i] * kernelEdge[i];
    vec3 colBlur = vec3(0.0);
    for(int i = 0; i < 9; i++)
        colBlur += sampleTex[i] * kernelBlur[i];
    vec3 value = texture(sampler,texCoord0).rgb;
    if (inverted == 1.0) {
        value = vec3(1.0)-value;
    }
    float edge = (colEdge.r+colEdge.g+colEdge.b)/3.0;
    if (edge > 0.001) {
        edge = 1.0;
        isEdge = true;
        gl_FragColor = vec4(value-vec3(0.3),1.0);
    }else {
        gl_FragColor = vec4(value,1.0);
    }

    //gl_FragColor = vec4(vec3(edge),1.0);
}
