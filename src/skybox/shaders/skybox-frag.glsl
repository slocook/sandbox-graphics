#version 430 core

in vec3 frag_normal ;
in vec3 frag_position ;
in vec3 frag_tex_coords ;

uniform samplerCube skybox_texture ;

out vec4 color ;

void main()
{
    color = texture(skybox_texture, frag_tex_coords) ;
}
