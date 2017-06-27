#version 430 core

layout( location = 0 ) in vec3 position ;
layout( location = 1 ) in vec2 tex_coords ;

out vec2 frag_tex_coords ;

uniform mat4 mvp_matrix ;

void main()
{
    frag_tex_coords = tex_coords ;
    gl_Position = mvp_matrix * vec4( position, 1.0 ) ;
}
