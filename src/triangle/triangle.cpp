#include "sandbox.h"

class triangle : public sandbox
{
    GLuint program ;
    GLuint vao ;

    void init()
    {
        const GLchar *vs_fname = "@CURR_PATH@/shaders/vert-shader.glsl" ;
        const GLchar *fs_fname = "@CURR_PATH@/shaders/frag-shader.glsl" ;

        std::list<std::pair<const char *, GLuint>> shaders ;
        shaders.push_back( std::make_pair( vs_fname, GL_VERTEX_SHADER ) ) ;
        shaders.push_back( std::make_pair( fs_fname, GL_FRAGMENT_SHADER ) ) ;

        program = loadShaders( shaders ) ;

        glGenVertexArrays( 1, &vao ) ;
        glBindVertexArray( vao ) ;
    }

    void render( double time )
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f } ;
        glClearBufferfv( GL_COLOR, 0, black ) ;

        glUseProgram( program ) ;

        glBindVertexArray( vao ) ;
        glDrawArrays( GL_TRIANGLES, 0, 3 ) ;
    }

    void shutdown()
    {
        glDeleteVertexArrays( 1, &vao ) ;
        glDeleteProgram( program ) ;
    }
} ;

MAIN( triangle, "Triangle" )
