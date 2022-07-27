#ifndef HEARTS_H_
#define HEARTS_H_
#include <ngl/Transformation.h>
#include <SDL2/SDL.h>

class Hearts
{
    public:
    
        Hearts(); 

        void draw();
        void resize(int _w, int _h);

    private:

        void loadMatricesToShader();

        bool m_translate;

        ngl::Transformation m_transform;
        ngl::Mat4 m_mouseGlobalTX;

        ngl::Mat4 m_view;
        ngl::Mat4 m_project;

};

#endif