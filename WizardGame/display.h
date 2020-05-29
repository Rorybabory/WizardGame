#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

#include <SDL2/SDL.h>
class Display
{
    public:
        Display(int width, int height, const std::string& title);
        void Update();
        void DrawText();
        void BindAsRenderTarget();
        void Clear(float r, float g, float b, float a);
        bool isClosed();
        virtual ~Display();
        SDL_Window* m_window;
        int width;
        int height;
        SDL_GLContext m_glContext;

    protected:
    private:
        Display(const Display& other);
        Display& operator=(const Display& other);

        bool m_isClosed;
};

#endif // DISPLAY_H
