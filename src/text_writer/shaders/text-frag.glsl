#version 430 core

in vec2 tex_coords ;

out vec4 color ;

uniform sampler2D text ;
uniform vec3 tex_color ;

void main()
{
    float alpha = texture( text, tex_coords ).r ;
    color = vec4( tex_color, alpha ) ;
}
