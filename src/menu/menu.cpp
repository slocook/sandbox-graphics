#include "menu.h"
#include "text_writer.h"
#include "sb_math.h"

const GLfloat vertices[6][3] =
{
    {-0.5f,  0.5f, 0.5f },
    {-0.5f, -0.5f, 0.5f },
    { 0.5f, -0.5f, 0.5f },

    {-0.5f,  0.5f, 0.5f },
    { 0.5f, -0.5f, 0.5f },
    { 0.5f,  0.5f, 0.5f }
} ;

Menu::Menu()
{
    vs_fname = "@CURR_PATH@/shaders/menu-vert.glsl" ;
    fs_fname = "@CURR_PATH@/shaders/menu-frag.glsl" ;

    glGenVertexArrays( 1, &vao ) ;
    glBindVertexArray( vao ) ;

    glGenBuffers( 1, &vbo ) ;
    glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), 0, GL_STATIC_DRAW ) ;
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices ) ;

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 ) ;
    glEnableVertexAttribArray( 0 ) ;
}

void Menu::render()
{
    GLint poly_mode ;
    glGetIntegerv( GL_POLYGON_MODE, &poly_mode ) ;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) ;

    glUseProgram( program ) ;
    glDisable( GL_DEPTH_TEST ) ;
    glEnable( GL_BLEND ) ;
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

    glBindVertexArray( vao ) ;
    glBindBuffer( GL_ARRAY_BUFFER, vbo ) ;

    // Draw quad
    glDrawArrays( GL_TRIANGLES, 0, 6 ) ;

    TextWriter *writer = TextWriter::getInstance() ;

    // Draw Title
    writer -> write( "Menu", 340.0f, 400.0f, 1.0f, glm::vec3( 0.0f, 0.0f, 0.0f ) ) ;

    // Draw Items
    float top = 350.0f ;
    float left = 225.0f ;
    float right = 450.0f ;
    for( auto item : item_list )
    {
        writer -> write( item.name, left, top, 0.5f, glm::vec3( 0.0f, 0.0f, 0.0f ) ) ;

        switch( item.type )
        {
            case TOGGLE :
                // TODO: Draw toggle boxes
                break ;
            default :
                break ;
        }

        top -= 30.0f ;
    }

    glPolygonMode( GL_FRONT_AND_BACK, poly_mode ) ;
}

void Menu::click( double xpos, double ypos )
{
    float left = 225.0f ;
    float right = 340.0f ;
    float top = 235.0f ;
    float bottom = 250.0f ;

    for( auto item : item_list )
    {
        // TODO: Check for clicks on the toggle boxes
        if( xpos < right && xpos > left && ypos > top && ypos < bottom )
        {
            if( item.type == TOGGLE )
            {
                *((bool*) item.data) = !*((bool*) item.data) ;
            }

            item.callback() ;
        }

        top -= 30.0f ;
        bottom -= 30.0f ;
    }
}

void Menu::addItem( const char *name, void (*callback)(void), MenuItemType type )
{
    MenuItem item ;
    item.name = name ;
    item.callback = callback ;
    item.type = type ;
    switch( item.type )
    {
        case TOGGLE :
            item.data = (void*) (new bool( false ) );
            break ;

        default :
            item.data = NULL ;
            break ;
    }

    item_list.push_back( item ) ;
}
