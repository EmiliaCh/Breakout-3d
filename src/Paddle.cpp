#include "Paddle.h"
#include <ngl/Transformation.h>
#include <SDL2/SDL.h>

void Paddle::draw()
{

  glViewport( 0, 0, m_width, m_height );

  // clear the screen and depth buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  // draw

  loadMatricesToShader();

  ngl::Mat4 M;

  m_transform.reset(); //Paddle
  {
    m_transform.getMatrix();
    m_transform.setPosition(0.0f,0.2f,0.0f); //Set start position of paddle 
    m_transform.setScale(2.0f, 0.3f, 1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
  } // and before a pop
  
}