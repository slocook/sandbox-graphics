#include "sandbox.h"

static const GLfloat vertices[] =
{
    0.95, -0.933, 0.5, 1.0,
   -0.95, -0.933, 0.5, 1.0,
    0.95,  0.933, 0.5, 1.0
} ;

class tessellation : public sandbox
{
    GLuint program ;
    GLuint vao ;
    GLuint vbo ;

    void init()
    {
        const GLchar *vs_fname  = "@CURR_PATH@/shaders/vert-shader.glsl" ;
        const GLchar *tcs_fname = "@CURR_PATH@/shaders/tesscon-shader.glsl" ;
        const GLchar *tes_fname = "@CURR_PATH@/shaders/tesseval-shader.glsl" ;
        const GLchar *fs_fname  = "@CURR_PATH@/shaders/frag-shader.glsl" ;

        std::list<std::pair<const char *, GLuint>> shaders ;
        shaders.push_back( std::make_pair( vs_fname, GL_VERTEX_SHADER ) ) ;
        shaders.push_back( std::make_pair( tcs_fname, GL_TESS_CONTROL_SHADER ) ) ;
        shaders.push_back( std::make_pair( tes_fname, GL_TESS_EVALUATION_SHADER ) ) ;
        shaders.push_back( std::make_pair( fs_fname, GL_FRAGMENT_SHADER ) ) ;

        program = loadShaders( shaders ) ;

        glGenVertexArrays( 1, &vao ) ;
        glBindVertexArray( vao ) ;

        glGenBuffers( 1, &vbo ) ;
        glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;
        glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW ) ;

        glEnableVertexAttribArray( 0 ) ;
        glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0 ) ;

        glPatchParameteri( GL_PATCH_VERTICES, 3 ) ;
    }

    void render( double time )
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f } ;
        glClearBufferfv( GL_COLOR, 0, black ) ;

        glUseProgram( program ) ;

        glBindVertexArray( vao ) ;
        glDrawArrays( GL_PATCHES, 0, 3 ) ;
    }

    void shutdown()
    {
        glDeleteVertexArrays( 1, &vao ) ;
        glDeleteBuffers( 1, &vbo ) ;
        glDeleteProgram( program ) ;
    }
} ;

MAIN( tessellation, "Tessellation" )
