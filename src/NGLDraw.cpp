#include "NGLDraw.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>

const static float INCREMENT=0.01f;
const static float ZOOM=0.05f;
constexpr auto PBR="PBR";
constexpr static float s_paddleUpdate=0.2f;

NGLDraw::NGLDraw()
{

  m_ballVelocity = ngl::Vec3(1,1,0);
  m_ballVelocity.normalize();
  m_ballVelocity *= 0.05;
  m_animate=true;
  m_rotate=true;


  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	   // Black Background Colour

  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);


  constexpr auto vertexShader  = "PBRVertex";
  constexpr auto fragShader    = "PBRFragment";

  // create shader program
  ngl::ShaderLib::createShaderProgram( PBR );

  // create empty shaders for Frag and Vert
  ngl::ShaderLib::attachShader( vertexShader, ngl::ShaderType::VERTEX );
  ngl::ShaderLib::attachShader( fragShader, ngl::ShaderType::FRAGMENT );

  ngl::ShaderLib::loadShaderSource( vertexShader, "shaders/PBRVertex.glsl" );
  ngl::ShaderLib::loadShaderSource( fragShader, "shaders/PBRFragment.glsl" );

  // compile the shaders
  ngl::ShaderLib::compileShader( vertexShader );
  ngl::ShaderLib::compileShader( fragShader );

  // adds them to the program
  ngl::ShaderLib::attachShaderToProgram( PBR, vertexShader );
  ngl::ShaderLib::attachShaderToProgram( PBR, fragShader );

  ngl::ShaderLib::linkProgramObject( PBR );

  ngl::ShaderLib::use(PBR);

 // view matrix for a static camera
  ngl::Vec3 from( 0.0f, 0.0f, 7.0f );
  ngl::Vec3 to( 0.0f, 1.5f, 0.0f );
  ngl::Vec3 up( 0.0f, 5.0f, 0.0f );

  // load to our new camera
  m_view=ngl::lookAt(from,to,up);
  ngl::ShaderLib::setUniform( "camPos", from );

  // default shader material and light porerties
  // these are "uniform" so will retain their values
  ngl::ShaderLib::setUniform("lightPosition",0.0f,2.0f,2.0f);
  ngl::ShaderLib::setUniform("lightColor",500.0f,80.0f,200.0f);
  ngl::ShaderLib::setUniform("exposure",2.2f);
  ngl::ShaderLib::setUniform("albedo",0.950f, 0.71f, 0.29f);

  ngl::ShaderLib::setUniform("metallic",1.02f);
  ngl::ShaderLib::setUniform("roughness",0.5f);
  ngl::ShaderLib::setUniform("ao",0.2f);
  ngl::VAOPrimitives::createTrianglePlane("floor",20,20,1,1,ngl::Vec3::up());
  ngl::VAOPrimitives::createSphere("sphere",0.5f,50);

  ngl::ShaderLib::use(ngl::nglCheckerShader); //Floor properites
  ngl::ShaderLib::setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);
  ngl::ShaderLib::setUniform("checkOn",true);
  ngl::ShaderLib::setUniform("lightPos",0.0f,2.0f,2.0f);
  ngl::ShaderLib::setUniform("colour1",1.9f,0.9f,1.9f,1.0f);
  ngl::ShaderLib::setUniform("colour2",0.0f,0.0f,0.0f,0.0f);
  ngl::ShaderLib::setUniform("checkSize",50.0f); 

  
}

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

  if (m_ballTransform.getPosition().m_x - m_radius <= 0 )
  {
      m_ballTransform.setPosition(m_ballTransform.getPosition().m_x*-1, m_ballTransform.getPosition().m_y, m_ballTransform.getPosition().m_z);
      m_ballVelocity.m_x*= -1.0;
  }
  if (m_ballTransform.getPosition().m_x + m_radius >= m_width)
  {
    m_ballTransform.setPosition(m_ballTransform.getPosition().m_x*-1, m_ballTransform.getPosition().m_y, m_ballTransform.getPosition().m_z);
    m_ballVelocity.m_x*= -1.0;
  }
  if (m_ballTransform.getPosition().m_y - m_radius <= 0)
  {
    m_ballTransform.setPosition(m_ballTransform.getPosition().m_x, m_ballTransform.getPosition().m_y*-1, m_ballTransform.getPosition().m_z);
    m_ballVelocity.m_y*= -1.0;
  }
  if (m_ballTransform.getPosition().m_y + m_radius >= m_height)
  {
    m_ballTransform.setPosition(m_ballTransform.getPosition().m_x, m_ballTransform.getPosition().m_y*-1, m_ballTransform.getPosition().m_z);
    m_ballVelocity.m_y*= -1.0;
  }

  m_ballTransform.addPosition(m_ballVelocity);


  // draw

  loadMatricesToShader();

  ngl::Mat4 M;
  

  m_transform.reset(); //Paddle
  {
    m_transform.getMatrix();
    m_transform.setPosition(m_paddleTransform.getPosition()); //Set start position of paddle 
    m_transform.setScale(2.0f, 0.3f, 1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");

  } // and before a pop

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(-4.0f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(-2.5f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(-1.0f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(0.5f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(2.0f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick
  {
    m_transform.getMatrix();
    m_transform.setPosition(3.5f,4.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick2
  {
    m_transform.getMatrix();
    m_transform.setPosition(3.5f,3.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick2
  {
    m_transform.getMatrix();
    m_transform.setPosition(2.0f,3.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop


  m_transform.reset(); //Brick2
  {
    m_transform.getMatrix();
    m_transform.setPosition(0.5f,3.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop


  m_transform.reset(); //Brick2
  {
    m_transform.getMatrix();
    m_transform.setPosition(-1.0f,3.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick2
  {
    m_transform.getMatrix();
    m_transform.setPosition(-2.5f,3.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop

  m_transform.reset(); //Brick2
  {
    m_transform.getMatrix();
    m_transform.setPosition(-4.0f,3.0f,0.0f); //Start of brick placement 
    m_transform.setScale(1.1f,0.4f,1.0f);
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("cube");
    /*brick array*/

  } // and before a pop


  m_transform.reset(); //Ball
  {

    m_transform.getMatrix();
    m_transform.setPosition(m_ballTransform.getPosition()); //Set start position
    m_transform.setScale(0.2f,0.2f,0.2f); //Makes it smaller

    
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("sphere");
  } //and before a pop

  ngl::ShaderLib::use(ngl::nglCheckerShader);
  ngl::Mat4 tx;
  tx.translate(0.0f,-0.55f,0.0f);
  ngl::Mat4 MVP=m_project*m_view*m_mouseGlobalTX*tx;
  ngl::Mat3 normalMatrix=m_view*m_mouseGlobalTX;
  normalMatrix.inverse().transpose();
  ngl::ShaderLib::setUniform("MVP",MVP);
  ngl::ShaderLib::setUniform("normalMatrix",normalMatrix);
  ngl::VAOPrimitives::draw("floor"); //Floor


}

//move ball
void NGLDraw::ballMove()
{
  // store the last position
  m_lastPos=m_pos;
  // update the current position
  m_pos+=m_dir;
  // get the next position
  m_nextPos=m_pos+m_dir;

}


void NGLDraw::loadMatricesToShader()
{
   ngl::ShaderLib::use("PBR");
   struct transform
   {
     ngl::Mat4 MVP;
     ngl::Mat4 normalMatrix;
     ngl::Mat4 M;
     
   };

    transform t;
    t.M=m_view*m_mouseGlobalTX*m_transform.getMatrix();

    t.MVP=m_project*t.M;
    t.normalMatrix=t.M;
    t.normalMatrix.inverse().transpose();
    ngl::ShaderLib::setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);

}


void NGLDraw::paddleMoveRight()
{
  m_paddleTransform.addPosition(ngl::Vec3(1,0,0)); 
}


void NGLDraw::paddleMoveLeft()
{
  m_paddleTransform.addPosition(ngl::Vec3(-1,0,0));
}


//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseMoveEvent (const SDL_MouseMotionEvent &_event)
{
  if(m_rotate && _event.state &SDL_BUTTON_LMASK)
  {
    int diffx=_event.x-m_origX;
    int diffy=_event.y-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event.x;
    m_origY = _event.y;
    this->draw();

  }
  // right mouse translate code
  else if(m_translate && _event.state &SDL_BUTTON_RMASK)
  {
    int diffX = (int)(_event.x - m_origXPos);
    int diffY = (int)(_event.y - m_origYPos);
    m_origXPos=_event.x;
    m_origYPos=_event.y;
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    this->draw();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mousePressEvent (const SDL_MouseButtonEvent &_event)
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event.button == SDL_BUTTON_LEFT)
  {
    m_origX = _event.x;
    m_origY = _event.y;
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event.button == SDL_BUTTON_RIGHT)
  {
    m_origXPos = _event.x;
    m_origYPos = _event.y;
    m_translate=true;
  }
}


//----------------------------------------------------------------------------------------------------------------------


void NGLDraw::mouseReleaseEvent (const SDL_MouseButtonEvent &_event)
{
  // this event is called when the mouse button is released

  if (_event.button == SDLK_a)
  {
    m_paddleTransform.addPosition(ngl::Vec3(0,0,0));
  }

  if (_event.button == SDLK_d)
  {
    m_paddleTransform.addPosition(ngl::Vec3(0,0,0));
  }
}

//----------------------------------------------------------------------------------------------------------------------


void NGLDraw::wheelEvent(const SDL_MouseWheelEvent &_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event.y > 0)
  {
    m_modelPos.m_z+=ZOOM;
    this->draw();
  }
  else if(_event.y <0 )
  {
    m_modelPos.m_z-=ZOOM;
    this->draw();
  }

  // check the diff of the wheel position (0 means no change)
  if(_event.x > 0)
  {
    m_modelPos.m_x-=ZOOM;
    this->draw();
  }
  else if(_event.x <0 )
  {
    m_modelPos.m_x+=ZOOM;
    this->draw();
  }
}
//----------------------------------------------------------------------------------------------------------------------
