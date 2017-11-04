#version 430 core

layout( location = 0 ) in vec3 position ;
layout( location = 1 ) in vec3 normal   ;

out vec3 frag_normal ;
out vec3 frag_position ;
out vec3 frag_tex_coords ;

uniform mat4 model_matrix ;
uniform mat4 mvp_matrix ;

void main()
{
    frag_normal     = vec3(model_matrix * vec4(normal,1.0) ) ;
    frag_position   = vec3(model_matrix * vec4(position, 1.0 ) ) ;
    frag_tex_coords = position ;

    gl_Position = mvp_matrix * vec4( position, 1.0 ) ;
}
