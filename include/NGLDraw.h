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
  
    void paddleMoveRight (const SDL_MouseButtonEvent &_event);

    void paddleMoveLeft (const SDL_MouseButtonEvent &_event);

    void wheelEvent(const SDL_MouseWheelEvent &_event);
    
  private :
    
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
    
    ngl::Vec3 m_modelPos;
    
    int m_width;
    int m_height;

    void movePaddle();

};


#endif
