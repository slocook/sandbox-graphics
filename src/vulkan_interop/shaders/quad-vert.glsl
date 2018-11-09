#version 430 core

layout( location = 0 ) in vec2 pos ;
layout( location = 1 ) in vec2 tc ;

out vec2 tex_coords ;

void main()
{
    gl_Position = vec4( pos.x, pos.y, 0.0, 1.0 ) ;
    tex_coords = tc ;
}
