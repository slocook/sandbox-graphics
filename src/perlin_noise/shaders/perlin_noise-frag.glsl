#version 430 core

in vec2 frag_tex_coords ;

out vec4 color ;

uniform sampler2D noise_tex ;

void main()
{
    color = texture( noise_tex, frag_tex_coords ) ;
}
