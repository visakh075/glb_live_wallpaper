#include "Core/Window.h"

#include "Core/Wallpaper.h"
#include "Core/Input.h"

#include <iostream>

#include <X11/Xatom.h>

namespace
{

void setAtom(
    Display* dpy,
    Window win,
    const char* name,
    const char* value)
{
    Atom property =
        XInternAtom(dpy, name, False);

    Atom atom =
        XInternAtom(dpy, value, False);

    XChangeProperty(
        dpy,
        win,
        property,
        XA_ATOM,
        32,
        PropModeReplace,
        reinterpret_cast<unsigned char*>(&atom),
        1);
}

}

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
    destroy();
}

bool WindowManager::create()
{
    m_display = XOpenDisplay(nullptr);

    if(!m_display)
        return false;

    int screen =
        DefaultScreen(m_display);

    Window root =
        RootWindow(m_display, screen);

    m_width =
        DisplayWidth(m_display, screen);

    m_height =
        DisplayHeight(m_display, screen);

    int attr[] =
    {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE, 24,
        None
    };

    XVisualInfo* vi =
        glXChooseVisual(
            m_display,
            screen,
            attr);

    if(!vi)
        return false;

    Colormap cmap =
        XCreateColormap(
            m_display,
            root,
            vi->visual,
            AllocNone);

    XSetWindowAttributes swa{};

    swa.colormap = cmap;

    swa.override_redirect = True;

    swa.event_mask =
        ExposureMask |
        StructureNotifyMask |
        KeyPressMask |
        KeyReleaseMask |
        ButtonPressMask |
        ButtonReleaseMask |
        PointerMotionMask;

    m_window =
        XCreateWindow(
            m_display,
            root,
            0,
            0,
            m_width,
            m_height,
            0,
            vi->depth,
            InputOutput,
            vi->visual,
            CWColormap |
            CWOverrideRedirect |
            CWEventMask,
            &swa);

    setAtom(
        m_display,
        m_window,
        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_WINDOW_TYPE_DESKTOP");

    Atom state =
        XInternAtom(
            m_display,
            "_NET_WM_STATE",
            False);

    Atom below =
        XInternAtom(
            m_display,
            "_NET_WM_STATE_BELOW",
            False);

    XChangeProperty(
        m_display,
        m_window,
        state,
        XA_ATOM,
        32,
        PropModeReplace,
        reinterpret_cast<unsigned char*>(&below),
        1);

    XMapWindow(
        m_display,
        m_window);

    XLowerWindow(
        m_display,
        m_window);

    m_context =
        glXCreateContext(
            m_display,
            vi,
            nullptr,
            True);

    if(!m_context)
        return false;

    glXMakeCurrent(
        m_display,
        m_window,
        m_context);

    XFree(vi);

    return true;
}

void WindowManager::destroy()
{
    if(m_context)
    {
        glXMakeCurrent(
            m_display,
            None,
            nullptr);

        glXDestroyContext(
            m_display,
            m_context);

        m_context = nullptr;
    }

    if(m_window)
    {
        XDestroyWindow(
            m_display,
            m_window);

        m_window = 0;
    }

    if(m_display)
    {
        XCloseDisplay(
            m_display);

        m_display = nullptr;
    }
}

bool WindowManager::running() const
{
    return m_running;
}

void WindowManager::close()
{
    m_running = false;
}

void WindowManager::pollEvents(
    Wallpaper& effect)
{
    while(XPending(m_display))
    {
        XEvent event;

        XNextEvent(
            m_display,
            &event);

        switch(event.type)
        {
            case DestroyNotify:

                m_running = false;

                break;

            case ConfigureNotify:

                m_width =
                    event.xconfigure.width;

                m_height =
                    event.xconfigure.height;

                break;

            default:
                break;
        }

        Input::processEvent(
            event,
            *this,
            effect);
    }
}

void WindowManager::swapBuffers()
{
    glXSwapBuffers(
        m_display,
        m_window);
}

int WindowManager::width() const
{
    return m_width;
}

int WindowManager::height() const
{
    return m_height;
}

Display* WindowManager::display() const
{
    return m_display;
}

Window WindowManager::handle() const
{
    return m_window;
}

GLXContext WindowManager::context() const
{
    return m_context;
}