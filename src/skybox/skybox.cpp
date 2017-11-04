#include "sandbox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const float skybox_vertices[24][6] =
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

const GLushort skybox_indices[] =
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

class skybox : public sandbox
{
    GLuint vao ;
    GLuint vbo ;
    GLuint ebo ;

    const char *front  = "@CURR_PATH@/textures/negz.jpg" ;
    const char *back   = "@CURR_PATH@/textures/posz.jpg" ;
    const char *top    = "@CURR_PATH@/textures/negy.jpg" ;
    const char *bottom = "@CURR_PATH@/textures/posy.jpg" ;
    const char *left   = "@CURR_PATH@/textures/negx.jpg" ;
    const char *right  = "@CURR_PATH@/textures/posx.jpg" ;

    void init()
    {
        // Set up shaders
        const GLchar *vs_fname = "@CURR_PATH@/shaders/skybox-vert.glsl" ;
        const GLchar *fs_fname = "@CURR_PATH@/shaders/skybox-frag.glsl" ;

        std::list<std::pair<const char *, GLuint>> shaders ;
        shaders.push_back( std::make_pair( vs_fname, GL_VERTEX_SHADER ) ) ;
        shaders.push_back( std::make_pair( fs_fname, GL_FRAGMENT_SHADER ) ) ;

        program = loadShaders( shaders ) ;

        // Set up element array buffer
        glGenBuffers( 1, &ebo ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ) ;
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( skybox_indices ), skybox_indices, GL_STATIC_DRAW ) ;

        // Set up vertex data
        glGenVertexArrays( 1, &vao ) ;
        glBindVertexArray( vao ) ;

        glGenBuffers( 1, &vbo ) ;
        glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;
        glBufferData( GL_ARRAY_BUFFER, sizeof( skybox_vertices ), skybox_vertices, GL_STATIC_DRAW ) ;

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, NULL ) ;
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, (void*) (3 * sizeof( float ) ) ) ;
        glEnableVertexAttribArray( 0 ) ;
        glEnableVertexAttribArray( 1 ) ;

        create_skybox_texture() ;

        view_matrix = glm::lookAt( glm::vec3( 0.0, 0.0, 0.0 ),
                                   glm::vec3( 0.0, 0.0, 0.0 ),
                                   glm::vec3( 0.0, 1.0, 0.0 ) ) ;
        projection_matrix = glm::perspective( 45.0f, (GLfloat) width / height, 0.1f, 100.0f ) ;
    }

    GLuint create_skybox_texture()
    {
        GLuint tex ;
        glActiveTexture(GL_TEXTURE0) ;
        glGenTextures(1, &tex) ;

        load_side(tex, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front ) ;
        load_side(tex, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back  ) ;
        load_side(tex, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, top   ) ;
        load_side(tex, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, bottom) ;
        load_side(tex, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left  ) ;
        load_side(tex, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right ) ;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE) ;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) ;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) ;

        return tex ;
    }

    void load_side(GLuint tex, GLenum target, const char* fname)
    {
        int x, y, n ;
        unsigned char *image_data = stbi_load(fname, &x, &y, &n, 4) ;
        if( !image_data )
        {
            throw std::runtime_error(std::string("Could not open texture file ") + std::string(fname));
        }

        glTexImage2D( target,
                      0,
                      GL_RGBA,
                      x, y,
                      0,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      image_data) ;
        free(image_data) ;
    }

    void render( double time )
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

        // Set up matrices
        const glm::mat4 model_matrix = glm::mat4(1.0) ;
        const glm::mat4 mvp_matrix = projection_matrix * skybox_matrix * model_matrix ;


        // Set shader state
        glUseProgram( program ) ;

        GLint model_loc = glGetUniformLocation( program, "model_matrix" ) ;
        glUniformMatrix4fv( model_loc, 1, GL_FALSE, glm::value_ptr( model_matrix ) ) ;

        GLint mvp_loc = glGetUniformLocation( program, "mvp_matrix" ) ;
        glUniformMatrix4fv( mvp_loc, 1, GL_FALSE, glm::value_ptr( mvp_matrix ) ) ;

        glBindVertexArray( vao ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ) ;

        glDisable( GL_CULL_FACE ) ;
        glDisable( GL_DEPTH_TEST ) ;
        glDepthMask( GL_TRUE ) ;
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

MAIN( skybox, "Skybox" )
