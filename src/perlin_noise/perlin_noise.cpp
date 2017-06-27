#include "sandbox.h"
#include <time.h>
#include <cstdlib>

struct Vertex
{
    glm::vec3 position ;
    glm::vec2 tex_coord ;

    Vertex( glm::vec3 pos, glm::vec2 coord )
    {
        position = pos ;
        tex_coord = coord ;
    }
} ;

const Vertex vertices[4] =
{
    Vertex( glm::vec3( -1.0,  1.0, -1.0 ), glm::vec2( 0.0, 1.0 ) ),
    Vertex( glm::vec3( -1.0, -1.0, -1.0 ), glm::vec2( 0.0, 0.0 ) ),
    Vertex( glm::vec3(  1.0,  1.0, -1.0 ), glm::vec2( 1.0, 1.0 ) ),
    Vertex( glm::vec3(  1.0, -1.0, -1.0 ), glm::vec2( 1.0, 0.0 ) )
} ;

const GLushort indices[4] =
{
    0, 1, 2, 3
} ;

class perlin_noise : public sandbox
{
    GLuint vao ;
    GLuint vbo ;
    GLuint ebo ;

    GLuint tex ;

    const int size = 1024 ;
    const int octaves = 5 ;

    int permutation[256] ;

    void init()
    {
        // Set up shaders
        const GLchar *vs_fname = "@CURR_PATH@/shaders/perlin_noise-vert.glsl" ;
        const GLchar *fs_fname = "@CURR_PATH@/shaders/perlin_noise-frag.glsl" ;

        std::list<std::pair<const char *, GLuint>> shaders ;
        shaders.push_back( std::make_pair( vs_fname, GL_VERTEX_SHADER ) ) ;
        shaders.push_back( std::make_pair( fs_fname, GL_FRAGMENT_SHADER ) ) ;

        program = loadShaders( shaders ) ;

        glGenTextures( 1, &tex ) ;
        glActiveTexture( GL_TEXTURE0 ) ;
        glBindTexture( GL_TEXTURE_2D, tex ) ;
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) ;
        glUniform1i( glGetUniformLocation( program, "noise_tex" ), 0 ) ;
        updateTexture() ;

        // Set up element array buffer
        glGenBuffers( 1, &ebo ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ) ;
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW ) ;

        // Set up vertex data
        glGenVertexArrays( 1, &vao ) ;
        glBindVertexArray( vao ) ;

        glGenBuffers( 1, &vbo ) ;
        glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW ) ;

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, NULL ) ;
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, (void*) (3 * sizeof( float ) ) ) ;
        glEnableVertexAttribArray( 0 ) ;
        glEnableVertexAttribArray( 1 ) ;
    }

    void updateTexture()
    {
        srand( time( NULL ) ) ;
        for( int i = 0 ; i < 256 ; i++ )
        {
            permutation[i] = i ;
        }
        for( int i = 0 ; i < 256 ; i++ )
        {
            int index = rand() % 256 ;
            int tmp   = permutation[i] ;
            permutation[ i ] = permutation[ index ] ;
            permutation[ index ] = tmp ;
        }

        unsigned char *data = (unsigned char*) malloc( size * size * 4) ;
        for( int i = 0 ; i < size ; i++ )
        {
            for( int j = 0 ; j < size ; j++ )
            {
                float noise = octave_perlin( (float)i, (float)j, octaves, 0.5, 1.0 / 128.0 ) ;
                data[ 4 * (i * size + j) + 0 ] = (GLubyte) (noise * 255) ;
                data[ 4 * (i * size + j) + 1 ] = (GLubyte) (noise * 255) ;
                data[ 4 * (i * size + j) + 2 ] = (GLubyte) (noise * 255) ;
                data[ 4 * (i * size + j) + 3 ] = (GLubyte) 255 ;
            }
        }
        glActiveTexture( GL_TEXTURE0 ) ;
        glBindTexture( GL_TEXTURE_2D, tex ) ;
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL ) ;
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGBA, GL_UNSIGNED_BYTE, data ) ;
        free( data ) ;
    }

    void render( double time )
    {
        glClear( GL_COLOR_BUFFER_BIT ) ;

        // Set shader state
        glUseProgram( program ) ;

        // Set up matrix
        const glm::mat4 mvp_matrix = glm::mat4( 1.0 ) ;
        GLint mvp_loc = glGetUniformLocation( program, "mvp_matrix" ) ;
        glUniformMatrix4fv( mvp_loc, 1, GL_FALSE, glm::value_ptr( mvp_matrix ) ) ;

        glActiveTexture( GL_TEXTURE0 ) ;
        glBindTexture( GL_TEXTURE_2D, tex ) ;

        glBindVertexArray( vao ) ;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ) ;

        glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL ) ;
    }

    void shutdown()
    {
        glDeleteBuffers( 1, &vbo ) ;
        glDeleteBuffers( 1, &ebo ) ;
        glDeleteVertexArrays( 1, &vao ) ;
        glDeleteProgram( program ) ;
    }

    float octave_perlin( float x, float y, int octaves, float persistence, float base_frequency )
    {
        float total = 0.0 ;
        float frequency = base_frequency ;
        float amplitude = 1.0 ;
        float max_value = 0.0 ;
        int repeat = (int) (size * base_frequency) ;
        for( int i = 0 ; i < octaves ; i++ )
        {
            total += perlin( x * frequency, y * frequency, repeat * (1 << i) ) * amplitude ;
            max_value += amplitude ;
            amplitude *= persistence ;
            frequency *= 2.0 ;
        }
        return total / max_value ;
    }

    float perlin( float x, float y, int repeat )
    {
        x = fmod( x, repeat ) ;
        y = fmod( y, repeat ) ;

        int x_i = (int)x & 255 ;
        int y_i = (int)y & 255 ;

        float x_f = x - (int)x ;
        float y_f = y - (int)y ;

        float u = fade( x_f ) ;
        float v = fade( y_f ) ;

        int aa = permutation[ (permutation[    (x_i        ) % 256 ] +     y_i         ) % 256 ] ;
        int ab = permutation[ (permutation[    (x_i        ) % 256 ] + inc(y_i, repeat)) % 256 ] ;
        int ba = permutation[ (permutation[ inc(x_i, repeat) % 256 ] +     y_i         ) % 256 ] ;
        int bb = permutation[ (permutation[ inc(x_i, repeat) % 256 ] + inc(y_i, repeat)) % 256 ] ;

        float x1 = mix( grad( aa, x_f    , y_f     ),
                        grad( ba, x_f - 1, y_f     ),
                        u ) ;
        float x2 = mix( grad( ab, x_f    , y_f - 1 ),
                        grad( bb, x_f - 1, y_f - 1 ),
                        u ) ;
        return ( mix( x1, x2, v ) + 1.0 ) / 2.0 ;
    }

    int inc( int num, int repeat )
    {
        return (num+1) % repeat ;
    }

    inline float mix( float a, float b, float t )
    {
        return a + t * (b - a) ;
    }

    float grad( int hash, float x, float y )
    {
        switch( hash & 0x3 )
        {
            case 0x0 : return  x + y ;
            case 0x1 : return -x + y ;
            case 0x2 : return  x - y ;
            case 0x3 : return -x - y ;
        }
    }

    inline float fade( double t )
    {
        return t * t * t * (t * (t * 6 - 15) + 10) ;
    }

    void handleInput( GLFWwindow *window, int key, int scancode, int action, int mode )
    {
        if( action == GLFW_PRESS )
        {
            if( key == GLFW_KEY_U )
                updateTexture() ;
        }
    }
} ;

MAIN( perlin_noise, "Perlin Noise" )
