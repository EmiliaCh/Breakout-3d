#ifndef NGLDRAW_H_
#define NGLDRAW_H_

#include <SDL2/SDL.h>
#include <ngl/Mat4.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>

class NGLDraw
{
  public :
   
    NGLDraw();
    
    ~NGLDraw();
   
    void resize(int _w, int _h);
   
    void draw();
   
    void mouseMoveEvent (const SDL_MouseMotionEvent &_event);
   
    void mousePressEvent (const SDL_MouseButtonEvent &_event);
    
    void mouseReleaseEvent (const SDL_MouseButtonEvent &_event);
  
    void paddleMoveRight ();

    void paddleMoveLeft ();

    void ballMove ();

    void wheelEvent(const SDL_MouseWheelEvent &_event);
    
  private :
    
        /*! the position of the sphere */
    ngl::Vec3 m_pos;
    /*! the radius of the sphere */
    float m_radius;
    /*! flag to indicate if the sphere has been hit by ray */
    bool m_hit;
    // the direction of the sphere
    ngl::Vec3 m_dir;
    // the last position of the sphere
    ngl::Vec3 m_lastPos;
    // the next position of the sphere
    ngl::Vec3 m_nextPos;

    void loadMatricesToShader();
    
    int m_spinXFace;
    int m_spinYFace;
    
    bool m_rotate;
    bool m_translate;
   
    int m_origX;
    int m_origY;
    int m_origXPos;
    int m_origYPos;
   
    ngl::Vec4 m_lightPos;
    ngl::Real m_lightAngle;

    float m_rotation;
    
    ngl::Transformation m_transform;
    ngl::Mat4 m_mouseGlobalTX;
   
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    ngl::Transformation m_paddleTransform;
    ngl::Transformation m_ballTransform;
    ngl::Vec3 m_ballVelocity;
   
    bool m_animate;
    int m_rayUpdateTimer;
    
    ngl::Vec3 m_modelPos;
    
    int m_width;
    int m_height;
    


};


#endif
