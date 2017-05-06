#include "text_writer.h"

#include <GL/glew.h>

#include <utility>

#define WIDTH 800
#define HEIGHT 600

TextWriter::TextWriter()
{
    loadCharacters() ;

    window_width  = WIDTH ;
    window_height = HEIGHT ;

    vs_fname = "@CURR_PATH@/shaders/text-vert.glsl" ;
    fs_fname = "@CURR_PATH@/shaders/text-frag.glsl" ;

    glGenVertexArrays( 1, &vao ) ;
    glBindVertexArray( vao ) ;

    glGenBuffers( 1, &vbo ) ;
    glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;

    // Two triangles of four floats each
    glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 6 * 4, NULL, GL_DYNAMIC_DRAW ) ;
    
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 ) ;
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (2 * 6 * sizeof( GLfloat )) ) ;
    glEnableVertexAttribArray( 0 ) ;
    glEnableVertexAttribArray( 1 ) ;

    glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;
    glBindVertexArray( 0 ) ;
}

void TextWriter::loadCharacters()
{
    FT_Library ft ;
    if( FT_Init_FreeType( &ft ) )
    {
        throw std::runtime_error( "Could not initialize FreeType!") ;
    }

    FT_Face face ;
    if( FT_New_Face( ft, "@PROJECT_ROOT@/fonts/ubuntu.ttf", 0, &face ) )
    {
        throw std::runtime_error( "Failed to load font!" ) ;
    }

    FT_Set_Pixel_Sizes( face, 0, 48 ) ;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) ;

    // Only generating ASCII characters for now
    for( GLubyte c = 0 ; c < 128 ; c++ )
    {
        if( FT_Load_Char( face, c, FT_LOAD_RENDER ) )
        {
            printf("Error: FreeType failed to load glyph %d!\n", c) ;
            continue ;
        }

        GLuint texture ;
        glGenTextures( 1, &texture ) ;
        glBindTexture( GL_TEXTURE_2D, texture ) ;
        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      GL_RED,
                      face -> glyph -> bitmap.width,
                      face -> glyph -> bitmap.rows,
                      0,
                      GL_RED,
                      GL_UNSIGNED_BYTE,
                      face -> glyph -> bitmap.buffer ) ;

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;

        TextChar character =
        {
            texture,
            { face -> glyph -> bitmap.width, face -> glyph -> bitmap.rows },
            { face -> glyph -> bitmap_left,  face -> glyph -> bitmap_top },
            (GLuint) face -> glyph -> advance.x
        } ;

        text_chars.insert( std::pair<GLchar, TextChar>(c, character) ) ;
    }

    FT_Done_Face( face ) ;
    FT_Done_FreeType( ft ) ;
}

void TextWriter::write( const char *text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color )
{
    if( text == nullptr ) return ;

    static glm::mat4 projection = glm::ortho( 0.0f, (float)window_width, 0.0f, (float)window_height ) ;

    GLint poly_mode ;
    glGetIntegerv( GL_POLYGON_MODE, &poly_mode ) ;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) ;

    glEnable( GL_BLEND ) ;
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

    glUseProgram( program ) ;
    
    GLuint proj_loc = glGetUniformLocation( program, "projection" ) ;
    glUniformMatrix4fv( proj_loc, 1, GL_FALSE, glm::value_ptr(projection) ) ;

    GLuint color_loc = glGetUniformLocation( program, "tex_color" ) ;
    glUniform3f( color_loc, color.x, color.y, color.z ) ;

    glActiveTexture( GL_TEXTURE0 ) ;

    glBindVertexArray( vao ) ;

    int index = 0 ;
    while( text[index] != 0 )
    {
        TextChar tc = text_chars[ text[index++] ] ;

        GLfloat xpos = x + tc.bearing[0] * scale ;
        GLfloat ypos = y - (tc.size[1] - tc.bearing[1]) * scale ;

        GLfloat w = tc.size[0] * scale ;
        GLfloat h = tc.size[1] * scale ;

        GLfloat positions[6][2] =
        {
            { xpos    , ypos + h },
            { xpos    , ypos     },
            { xpos + w, ypos     },

            { xpos    , ypos + h },
            { xpos + w, ypos     },
            { xpos + w, ypos + h }
        } ;

        GLfloat tex_coords[6][2] =
        {
            { 0.0, 0.0 },
            { 0.0, 1.0 },
            { 1.0, 1.0 },

            { 0.0, 0.0 },
            { 1.0, 1.0 },
            { 1.0, 0.0 }
        } ;

        glBindTexture( GL_TEXTURE_2D, tc.tex_id ) ;

        glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( positions ), positions ) ;
        glBufferSubData( GL_ARRAY_BUFFER, sizeof( positions ), sizeof( tex_coords ), tex_coords ) ;

        glDrawArrays( GL_TRIANGLES, 0, 6 ) ;

        x += (tc.advance >> 6) * scale ;
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) ;
}
