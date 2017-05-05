#ifndef __TEXT_H__
#define __TEXT_H__
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/vec3.hpp>

#include <map>
#include <string>

struct TextChar
{
    GLuint  tex_id     ;
    int32_t size[2]    ;
    int32_t bearing[2] ;
    GLuint  advance    ;
} ;

class TextWriter
{
public:
    std::string vs_fname ;
    std::string fs_fname ;

    TextWriter() ;
    ~TextWriter() ;

    void setProgram( GLuint _program ) { program = _program ; }

    void write( const char *text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color ) ;
private:
    std::map<GLchar, TextChar> text_chars ;

    GLuint vao ;
    GLuint vbo ;

    GLuint program ;

    void loadCharacters() ;
} ;

#endif
