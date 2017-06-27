#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "sb_math.h"

class Camera
{
public:
    glm::mat4 view_matrix ;
private:
    glm::vec3 position ;

    glm::vec3 right ;
    glm::vec3 up ;
    glm::vec3 forward ;

    float heading ;
    float pitch ;

    float velocity ;
    float sensitivity ;
    glm::vec3 direction ;

public:
    Camera()
    {
        position    = glm::vec3( 0.0f, 1.0f, 10.0f ) ;
        right       = glm::vec3( 1.0f, 0.0f, 0.0f ) ;
        up          = glm::vec3( 0.0f, 1.0f, 0.0f ) ;
        forward     = glm::vec3( 0.0f, 0.0f, -1.0f ) ;
        view_matrix = glm::lookAt( position, position + forward, up ) ;
        velocity    = 10.0f ;
        sensitivity = 0.1f ;
        direction   = glm::vec3( 0.0f, 0.0f, -1.0f ) ;
        heading     = 0.0 ;
        pitch       = 0.0 ;
    }

    ~Camera() {}

    void update( double time )
    {
        static float last_time = 0.0 ;
        float delta = time - last_time ;

        position += direction * velocity * delta ;

        view_matrix = glm::lookAt( position, position + forward, up ) ;

        last_time = time ;

        //printf("------------------------------------\n");
        //printf("position = (%f, %f, %f)\n", position.x, position.y, position.z) ;
        //printf("right    = (%f, %f, %f)\n", right.x, right.y, right.z) ;
        //printf("up       = (%f, %f, %f)\n", up.x, up.y, up.z) ;
        //printf("forward  = (%f, %f, %f)\n", forward.x, forward.y, forward.z) ;
        //printf("heading  = %f\n", heading) ;
        //printf("pitch    = %f\n", pitch) ;
    }

    void setOffset( double xoff, double yoff )
    {
        heading += xoff * sensitivity ;
        pitch   += -yoff * sensitivity ;

        if( pitch < -89.0 ) pitch = -89.0 ;
        if( pitch >  89.0 ) pitch =  89.0 ;

        right   = glm::vec3( 1.0,  0.0,  0.0 ) ;
        up      = glm::vec3( 0.0,  1.0,  0.0 ) ;
        forward = glm::vec3( 0.0,  0.0, -1.0 ) ;

        glm::mat3 rot = glm::mat3( glm::rotate( glm::mat4( 1.0 ), glm::radians( -heading ), up ) ) * 
                        glm::mat3( glm::rotate( glm::mat4( 1.0 ), glm::radians( pitch ), right ) ) ;

        forward = rot * forward ;
        right   = glm::normalize( glm::cross( forward, up ) ) ;
    }

    void setDirection( glm::vec3 dir )
    {
        if( abs(dir.x) < 0.01 && abs(dir.y) < 0.01 && abs(dir.z) < 0.01 )
            direction = glm::vec3( 0.0 ) ;
        else
            direction = glm::normalize( 
                            right   * dir.x +
                            up      * dir.y +
                            forward * dir.z ) ;
    }
} ;

#endif
