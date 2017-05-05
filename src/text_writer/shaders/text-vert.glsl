#version 430 core

layout( location = 0 ) in vec2 in_pos ;
layout( location = 1 ) in vec2 in_tex_coords ;

out vec2 tex_coords ;

uniform mat4 projection ;

void main()
{
    gl_Position = projection * vec4( in_pos, 0.0, 1.0 ) ;
    tex_coords = in_tex_coords ;
}
