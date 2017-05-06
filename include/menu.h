#ifndef __MENU_H__
#define __MENU_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <list>

enum MenuItemType
{
    TOGGLE,
    INPUT,
    NUM_TYPES
} ;

struct MenuItem
{
    const char *name ;
    void (*callback)() ;
    MenuItemType type ;
    void *data ;
} ;

class Menu
{
public:
    Menu() ;
    ~Menu() ;

    void render() ;

    void click( double xpos, double ypos ) ;

    void addItem( const char *name, void (*callback)(void), MenuItemType type ) ;

    void setProgram( GLuint _program ) { program = _program ; }

    std::list<std::pair<const char*,GLuint>> getShaders()
    {
        std::list<std::pair<const char*,GLuint>> shaders ;
        shaders.push_back( std::make_pair( vs_fname.c_str(), GL_VERTEX_SHADER ) ) ;
        shaders.push_back( std::make_pair( fs_fname.c_str(), GL_FRAGMENT_SHADER ) ) ;

        return shaders ;
    }

private:
    std::vector<MenuItem> item_list ;

    GLuint program ;

    GLuint vao ;
    GLuint vbo ;

    std::string vs_fname ;
    std::string fs_fname ;
} ;

#endif
