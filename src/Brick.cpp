#include "Brick.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>
#include <SDL2/SDL.h>


NGLDraw::~NGLDraw()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLDraw::resize(int _w, int _h)
{
  glViewport(0,0, _w  , _h );
  m_project=ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.1f, 200.0f );
  m_width=_w;
  m_height=_h;
}

void NGLDraw::draw()
{

  glViewport( 0, 0, m_width, m_height );

  // clear the screen and depth buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  // draw

  loadMatricesToShader();

  ngl::Mat4 M;

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(-2.0f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /**/

  } // and before a pop

} 

//basic array
void Brick::array() //progress
{

    size_t size = 6;
    std::vector<int&gt array(size);    
                                    // and initialize them to 0
    // do something with them:
    for(int i=0; i<size; ++i){
        array[i] = i;
    }

}

//alternate array??
void Brick::array()
{
  for (int i=0; j<BRICK_W; i++){
    for (int j=0; j<BRICK_H; j++){
      Brick brick;

      brick.state =true;
      brick [i][j] = brick;
    }
  }
}