#include "sandbox.h"

class triangle : public sandbox
{
    GLuint program ;
    GLuint vao ;

    void init()
    {
        const GLchar *vs_fname = "@CURR_PATH@/shaders/vert-shader.glsl" ;
        const GLchar *fs_fname = "@CURR_PATH@/shaders/frag-shader.glsl" ;

        program = loadShaders( vs_fname, fs_fname ) ;

        glGenVertexArrays( 1, &vao ) ;
        glBindVertexArray( vao ) ;
    }

    void render( double time )
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f } ;
        glClearBufferfv( GL_COLOR, 0, black ) ;

        glUseProgram( program ) ;

        glDrawArrays( GL_TRIANGLES, 0, 3 ) ;
    }

    void shutdown()
    {
        glDeleteVertexArrays( 1, &vao ) ;
        glDeleteProgram( program ) ;
    }
} ;

MAIN( triangle, "Triangle" )
