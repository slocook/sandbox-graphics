#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <stdexcept>
#include <string>

#include "text_writer.h"

class sandbox
{
public:
    sandbox() {}
    virtual ~sandbox()
    {
        glfwTerminate() ;
    }
    
    virtual void run( sandbox *_app, const char *name )
    {
        app = _app ;
        menu_active = false ;

        if( !glfwInit() )
        {
            throw std::runtime_error("Failed to initialize GLFW!") ;
        }

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 ) ;
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ) ;
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ) ;
        glfwWindowHint( GLFW_RESIZABLE, GL_FALSE ) ;

        window = glfwCreateWindow( 800, 600, name, nullptr, nullptr ) ;
        if( window == nullptr )
        {
            throw std::runtime_error("Failed to create GLFW window!") ;
        }
        glfwMakeContextCurrent( window ) ;

        glfwSetKeyCallback( window, keyCallback ) ;

        glewExperimental = GL_TRUE ;
        if( glewInit() != GLEW_OK )
        {
            throw std::runtime_error("Failed to initialize GLEW!") ;
        }

        glfwGetFramebufferSize( window, &width, &height ) ;

        glViewport( 0, 0, width, height ) ;

        text_writer = new TextWriter() ;
        text_writer -> setProgram( 
                loadShaders( (text_writer -> vs_fname).c_str(), (text_writer -> fs_fname).c_str() ) ) ;
        
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) ;

        init() ;

        while( !glfwWindowShouldClose( window ) )
        {
            glfwPollEvents() ;

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

            double start_time = glfwGetTime() ;

            render( start_time ) ;

            if( menu_active )
                renderMenu() ;

            if( show_framerate )
                renderFramerate() ;

            glfwSwapBuffers( window ) ;
        }

        cleanup() ;
    }

    void renderMenu()
    {
        
    }

    void renderFramerate()
    {
        static int frames = 0 ;
        static double last_time = 0.0 ;
        static char buffer[32] ;
        
        frames++;

        double now = glfwGetTime() ;

        if( now - last_time >= 1.0 )
        {
            memset( buffer, 0, 32 ) ;
            snprintf( buffer, 32, "%.2f ms", 1000.0/frames ) ;

            last_time += 1.0 ;
            frames = 0 ;
        }

        text_writer -> write( buffer, 20.0f, 570.0f, 0.4f, glm::vec3( 1.0 ) ) ;
    }

    const GLchar *readFile( const char *fname )
    {
        FILE *f = fopen( fname, "rb" ) ;

        if( !f )
        {
            throw std::runtime_error(std::string("Could not open shader file!") + std::string(fname)) ;
        }

        fseek( f, 0, SEEK_END ) ;
        long fsize = ftell( f ) ;
        fseek( f, 0, SEEK_SET ) ;

        GLchar *data = new GLchar[fsize + 1] ;
        fread( data, 1, fsize, f ) ;
        data[ fsize ] = 0 ;
        fclose( f ) ;

        return const_cast<const GLchar*>(data) ;
    }

    virtual GLuint loadShaders( const char *vs_fname,
                                const char *fs_fname )
    {
        // Read vertex and fragment shader sources
        const GLchar *vs_source = readFile( vs_fname ) ;
        const GLchar *fs_source = readFile( fs_fname ) ;

        if( vs_source == NULL || fs_source == NULL )
        {
            throw std::runtime_error("Empty shader?!?") ;
        }

        // Create the vertex shader
        GLuint vs = glCreateShader( GL_VERTEX_SHADER ) ;
        glShaderSource( vs, 1, &vs_source, NULL ) ;
        glCompileShader( vs ) ;

        // Create the fragment shader
        GLuint fs = glCreateShader( GL_FRAGMENT_SHADER ) ;
        glShaderSource( fs, 1, &fs_source, NULL ) ;
        glCompileShader( fs ) ;

        // Create the program
        GLuint program = glCreateProgram() ;

        glAttachShader( program, vs ) ;
        glAttachShader( program, fs ) ;

        glLinkProgram( program ) ;

        delete[] vs_source ;
        delete[] fs_source ;

        return program ;
    }

    virtual void init() {}
    virtual void render( double time ) {}
    virtual void cleanup() {}
    virtual void menu() {}
    virtual void handleInput( GLFWwindow *window, int key, int scancode, int action, int mode ) {}

    static void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
    {
        if( app )
            app -> handleInputMain( window, key, scancode, action, mode ) ;
    }

    void handleInputMain( GLFWwindow *window, int key, int scancode, int action, int mode )
    {
        if( action == GLFW_PRESS )
        {
            switch( key )
            {
                case GLFW_KEY_ESCAPE :
                    glfwSetWindowShouldClose( window, GL_TRUE ) ;
                    break ;

                case GLFW_KEY_F :
                    show_framerate = !show_framerate ;
                    break ;

                case GLFW_KEY_TAB :
                    menu_active = !menu_active ;
                    break ;

                default :
                    handleInput( window, key, scancode, action, mode ) ;
                    break ;
            }
        }
    }

protected:
    static sandbox *app ;

    GLFWwindow *window ;

    TextWriter *text_writer ;

    int width ;
    int height ;

    GLuint program ;
    GLuint text_program ;

    bool show_framerate ;
    bool menu_active ;
} ;

#define MAIN(a,name)                    \
int main( int argc, const char **argv ) \
{                                       \
    a *app = new a ;                    \
    app -> run( app, name ) ;           \
    delete app ;                        \
    return 0 ;                          \
}                                       \

#endif
