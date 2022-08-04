#ifdef WIN32
    #define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include "NGLDraw.h"
#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);



int main(int argc, char * argv[])
{
  //Paddle p1;

  NGL_UNUSED(argc);
  NGL_UNUSED(argv);
  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or error occurs
    SDLErrorExit("Unable to initialize SDL");
  }

  // create a window we need to init the video
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  // create window
  SDL_Window *window=SDL_CreateWindow("3D Breakout!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 
                                    SDL_WINDOW_OPENGL |  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
                                      
                                      
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context
   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context ");
   }
   // make this our current GL context
   SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);
  ngl::NGLInit::initialize();
  // now clear the screen and swap whilst NGL inits
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  bool quit=false;
  SDL_Event event;
  NGLDraw ngl;
  ngl::Transformation M;
  while(!quit)
  {

    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // process the mouse data by passing it to ngl class
        case SDL_MOUSEMOTION : ngl.mouseMoveEvent(event.motion); break;
        case SDL_MOUSEBUTTONDOWN : ngl.mousePressEvent(event.button); break;
        case SDL_MOUSEBUTTONUP : ngl.mouseReleaseEvent(event.button); break;
        case SDL_MOUSEWHEEL : ngl.wheelEvent(event.wheel); break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          #ifdef __APPLE__
            ngl.resize(w*2,h*2);
          #else
            ngl.resize(w,h);
          #endif
        break;

        // keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
 
            case SDLK_a :
              //m_paddleTransform.addPosition(ngl::Vec3(1,0,0));//will make paddle move in left direction
              break;

            case SDLK_d :  
              //m_paddleTransform.addPosition() +ngl::Vec3(1,0,0));//will make paddle move in right direction
              break;

            case SDLK_f :
            SDL_SetWindowFullscreen(window,SDL_TRUE); //Makes game fullscreen
            glViewport(0,0,rect.w*2,rect.h*2);
            break;

            case SDLK_g : SDL_SetWindowFullscreen(window,SDL_FALSE); break; //Makes player exit fullscreen
            default : break;
          } // end of key process
          break;
        } // end of keydown

        //key up event
        case SDL_KEYUP:
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_a: ngl.paddleMoveRight(true); break;
            case SDLK_d: ngl.paddleMoveLeft(true); break;
            //When a or d are realised paddle stops moving 
              //paddledir = 0; will make paddle stop moving 
              break;
          }//end of key process
          break;
        }//end of key up

        break;
      default : break;
      } // end of event switch
    } // end of poll events

    // now we draw ngl
    ngl.draw();
    // swap the buffers
    SDL_GL_SwapWindow(window);

  }
  // tidy up and exit SDL
 SDL_Quit();
 
 return EXIT_SUCCESS;
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
   
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // set multi sampling
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  // enable double buffering 
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
} 