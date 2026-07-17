#pragma once

#include <X11/Xlib.h>
#include <GL/glx.h>

class Effect;

class WindowManager
{
public:

    WindowManager();
    ~WindowManager();

    bool create();

    void destroy();

    bool running() const;

    void close();

    void pollEvents(Effect& effect);

    void swapBuffers();

    int width() const;
    int height() const;

    Display* display() const;
    Window handle() const;
    GLXContext context() const;

private:

    Display* m_display = nullptr;

    Window m_window = 0;

    GLXContext m_context = nullptr;

    int m_width = 0;
    int m_height = 0;

    bool m_running = true;
};