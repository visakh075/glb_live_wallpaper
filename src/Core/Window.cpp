#include "Core/Window.h"

#include "Core/Wallpaper.h"
#include "Core/Input.h"

#include <iostream>

#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>

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
        RootWindow(
            m_display,
            screen);

    m_width =
        DisplayWidth(
            m_display,
            screen);

    m_height =
        DisplayHeight(
            m_display,
            screen);

    int fbAttribs[] =
    {
        GLX_X_RENDERABLE , True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE  , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE     , 8,
        GLX_GREEN_SIZE   , 8,
        GLX_BLUE_SIZE    , 8,
        GLX_ALPHA_SIZE   , 8,
        GLX_DEPTH_SIZE   , 24,
        GLX_DOUBLEBUFFER , True,
        None
    };

    int fbcount = 0;

    GLXFBConfig* fbconfigs =
        glXChooseFBConfig(
            m_display,
            screen,
            fbAttribs,
            &fbcount);

    if(!fbconfigs)
        return false;

    GLXFBConfig best =
        nullptr;

    XVisualInfo* vi =
        nullptr;

    for(int i = 0;
        i < fbcount;
        ++i)
    {
        XVisualInfo* tmp =
            glXGetVisualFromFBConfig(
                m_display,
                fbconfigs[i]);

        if(!tmp)
            continue;

        XRenderPictFormat* fmt =
            XRenderFindVisualFormat(
                m_display,
                tmp->visual);

        if(fmt &&
           fmt->direct.alphaMask > 0)
        {
            best =
                fbconfigs[i];

            vi =
                tmp;

            break;
        }

        XFree(tmp);
    }

    XFree(fbconfigs);

    if(!vi)
    {
        std::cout
            << "No ARGB visual found\n";

        return false;
    }

    Colormap cmap =
        XCreateColormap(
            m_display,
            root,
            vi->visual,
            AllocNone);

    XSetWindowAttributes swa{};

    swa.colormap =
        cmap;

    swa.override_redirect =
        True;

    swa.background_pixel =
        0;

    swa.border_pixel =
        0;

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
            CWBackPixel |
            CWBorderPixel |
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
        reinterpret_cast<unsigned char*>(
            &below),
        1);

    XMapWindow(
        m_display,
        m_window);

    XLowerWindow(
        m_display,
        m_window);

    m_context =
        glXCreateNewContext(
            m_display,
            best,
            GLX_RGBA_TYPE,
            nullptr,
            True);

    if(!m_context)
        return false;

    glXMakeCurrent(
        m_display,
        m_window,
        m_context);

    glEnable(
        GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

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