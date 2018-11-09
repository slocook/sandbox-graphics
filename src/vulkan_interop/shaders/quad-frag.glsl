#version 430 core

out vec4 color ;

in vec2 tex_coords ;

uniform sampler2D screen_tex ;

void main()
{
    color = texture( screen_tex, tex_coords ) ;
}
