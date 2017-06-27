#include "sandbox.h"

    //{  10.0, 10.0, 10.0, 0.0, 0.0, 1.0 },
    //{  10.0, 10.0, 10.0, 0.0, 0.0, 1.0 },
    //{  10.0, 10.0, 10.0, 0.0, 0.0, 1.0 },
    //{  10.0, 10.0, 10.0, 0.0, 0.0, 1.0 },

const float cube_vertices[24][6] =
{
    // V_x   V_y   V_z   N_x   N_y   N_z
    // Front
    { -1.0, -1.0,  1.0,  0.0,  0.0,  1.0 },
    {  1.0, -1.0,  1.0,  0.0,  0.0,  1.0 },
    { -1.0,  1.0,  1.0,  0.0,  0.0,  1.0 },
    {  1.0,  1.0,  1.0,  0.0,  0.0,  1.0 },

    // Back
    {  1.0, -1.0, -1.0,  0.0,  0.0, -1.0 },
    { -1.0, -1.0, -1.0,  0.0,  0.0, -1.0 },
    {  1.0,  1.0, -1.0,  0.0,  0.0, -1.0 },
    { -1.0,  1.0, -1.0,  0.0,  0.0, -1.0 },

    // Right
    {  1.0, -1.0,  1.0,  1.0,  0.0,  0.0 },
    {  1.0, -1.0, -1.0,  1.0,  0.0,  0.0 },
    {  1.0,  1.0,  1.0,  1.0,  0.0,  0.0 },
    {  1.0,  1.0, -1.0,  1.0,  0.0,  0.0 },
    
    // Left
    { -1.0, -1.0, -1.0, -1.0,  0.0,  0.0 },
    { -1.0, -1.0,  1.0, -1.0,  0.0,  0.0 },
    { -1.0,  1.0, -1.0, -1.0,  0.0,  0.0 },
    { -1.0,  1.0,  1.0, -1.0,  0.0,  0.0 },

    // Bottom
    { -1.0, -1.0, -1.0,  0.0, -1.0,  0.0 },
    {  1.0, -1.0, -1.0,  0.0, -1.0,  0.0 },
    { -1.0, -1.0,  1.0,  0.0, -1.0,  0.0 },
    {  1.0, -1.0,  1.0,  0.0, -1.0,  0.0 },

    // Top
    { -1.0,  1.0,  1.0,  0.0,  1.0,  0.0 },
    {  1.0,  1.0,  1.0,  0.0,  1.0,  0.0 },
    { -1.0,  1.0, -1.0,  0.0,  1.0,  0.0 },
    {  1.0,  1.0, -1.0,  0.0,  1.0,  0.0 }
} ;

const GLushort cube_indices[] =
{
    0, 1, 2, 3,
    0xFFFF,
    4, 5, 6, 7,
    0xFFFF,
    8, 9, 10, 11,
    0xFFFF,
    12, 13, 14, 15,
    0xFFFF,
    16, 17, 18, 19,
    0xFFFF,
    20, 21, 22, 23
} ;

class cube : public sandbox
{
    GLuint vao ;
    GLuint vbo ;
    GLuint ebo ;

    void init()
    {
        // Set up shaders
        const GLchar *vs_fname = "@CURR_PATH@/shaders/cube-vert.glsl" ;
        const GLchar *fs_fname = "@CURR_PATH@/shaders/cube-frag.glsl" ;

        std::list<std::pair<const char *, GLuint>> shaders ;
        shaders.push_back( std::make_pair( vs_fname, GL_VERTEX_SHADER ) ) ;
        shaders.push_back( std::make_pair( fs_fname, GL_FRAGMENT_SHADER ) ) ;

        program = loadShaders( shaders ) ;

        // Set up element array buffer
        glGenBuffers( 1, &ebo ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ) ;
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cube_indices ), cube_indices, GL_STATIC_DRAW ) ;

        // Set up vertex data
        glGenVertexArrays( 1, &vao ) ;
        glBindVertexArray( vao ) ;

        glGenBuffers( 1, &vbo ) ;
        glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;
        glBufferData( GL_ARRAY_BUFFER, sizeof( cube_vertices ), cube_vertices, GL_STATIC_DRAW ) ;

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, NULL ) ;
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, (void*) (3 * sizeof( float ) ) ) ;
        glEnableVertexAttribArray( 0 ) ;
        glEnableVertexAttribArray( 1 ) ;

        view_matrix = glm::lookAt( glm::vec3( 0.0, 0.0, 8.0 ),
                                   glm::vec3( 0.0, 0.0, 0.0 ),
                                   glm::vec3( 0.0, 1.0, 0.0 ) ) ;
        projection_matrix = glm::perspective( 45.0f, (GLfloat) width / height, 0.1f, 100.0f ) ;
    }

    void render( double time )
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

        // Set up matrices
        const glm::mat4 model_matrix = glm::mat4(1.0) ;
        const glm::mat4 mvp_matrix = projection_matrix * view_matrix * model_matrix ;

        // Set shader state
        glUseProgram( program ) ;

        GLint model_loc = glGetUniformLocation( program, "model_matrix" ) ;
        glUniformMatrix4fv( model_loc, 1, GL_FALSE, glm::value_ptr( model_matrix ) ) ;

        GLint mvp_loc = glGetUniformLocation( program, "mvp_matrix" ) ;
        glUniformMatrix4fv( mvp_loc, 1, GL_FALSE, glm::value_ptr( mvp_matrix ) ) ;

        glBindVertexArray( vao ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ) ;

        glEnable( GL_CULL_FACE ) ;
        glEnable( GL_DEPTH_TEST ) ;
        glEnable( GL_PRIMITIVE_RESTART ) ;
        glPrimitiveRestartIndex( 0xFFFF ) ;

        glDrawElements( GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, NULL ) ;
    }

    void shutdown()
    {
        glDeleteBuffers( 1, &vbo ) ;
        glDeleteBuffers( 1, &ebo ) ;
        glDeleteVertexArrays( 1, &vao ) ;
        glDeleteProgram( program ) ;
    }
} ;

MAIN( cube, "Cube" )
