#version 430 core

in vec3 frag_normal ;
in vec3 frag_position ;

out vec4 color ;

void main()
{
    vec3 light_pos = vec3( 5.0, 10.0, 10.0 ) ;

    vec3 ambient = vec3( 0.1 ) * vec3( 1.0 ) ;

    vec3 norm = normalize( frag_normal ) ;
    vec3 light_dir = normalize( light_pos - frag_position ) ;

    float cos_t = max( dot( norm, light_dir ), 0.0 ) ;
    vec3 diffuse = vec3( cos_t ) ;

    color = vec4( vec3( 1.0, 0.0, 0.0 ) * (ambient + diffuse), 1.0 ) ;
}
