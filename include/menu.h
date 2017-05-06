#ifndef __MENU_H__
#define __MENU_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

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
    std::string vs_fname ;
    std::string fs_fname ;

    Menu() ;
    ~Menu() ;

    void render() ;

    void click( double xpos, double ypos ) ;

    void addItem( const char *name, void (*callback)(void), MenuItemType type ) ;

    void setProgram( GLuint _program ) { program = _program ; }

private:
    std::vector<MenuItem> item_list ;

    GLuint program ;

    GLuint vao ;
    GLuint vbo ;
} ;

#endif
