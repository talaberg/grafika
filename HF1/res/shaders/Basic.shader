#shader vertex
#version 330                // Shader 3.3
precision highp float;      // normal floats, makes no difference on desktop computers

uniform mat4 MVP;           // uniform variable, the Model-View-Projection transformation matrix
layout(location = 0) in vec2 vp;    // Varying input: vp = vertex position is expected in attrib array 0

void main()
{
    gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;        // transform vp from modeling space to normalized device space
}

#shader fragment
#version 330            // Shader 3.3
precision highp float;  // normal floats, makes no difference on desktop computers

uniform vec3 color;     // uniform variable, the color of the primitive
out vec4 outColor;      // computed color of the current pixel

void main()
{
    outColor = vec4(0, 1, 0, 1);    // computed color is the color of the primitive
}