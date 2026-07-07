#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <chrono>
#include <thread>

static void setAtom(Display *dpy,
                    Window win,
                    const char *name,
                    const char *value)
{
    Atom property = XInternAtom(dpy, name, False);
    Atom atom = XInternAtom(dpy, value, False);

    XChangeProperty(
        dpy,
        win,
        property,
        XA_ATOM,
        32,
        PropModeReplace,
        (unsigned char *)&atom,
        1);
}

int main()
{
    Display *dpy = XOpenDisplay(nullptr);
    if (!dpy)
        return 1;

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    int W = DisplayWidth(dpy, screen);
    int H = DisplayHeight(dpy, screen);

    int attr[] =
    {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE, 24,
        None
    };

    XVisualInfo *vi =
        glXChooseVisual(dpy, screen, attr);

    if (!vi)
        return 1;

    Colormap cmap =
        XCreateColormap(
            dpy,
            root,
            vi->visual,
            AllocNone);

    XSetWindowAttributes swa{};
    swa.colormap = cmap;
    swa.override_redirect = True;
    swa.event_mask = ExposureMask | StructureNotifyMask;

    Window win =
        XCreateWindow(
            dpy,
            root,
            0,
            0,
            W,
            H,
            0,
            vi->depth,
            InputOutput,
            vi->visual,
            CWColormap |
            CWOverrideRedirect |
            CWEventMask,
            &swa);

    setAtom(
        dpy,
        win,
        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_WINDOW_TYPE_DESKTOP");

    Atom state =
        XInternAtom(dpy, "_NET_WM_STATE", False);

    Atom below =
        XInternAtom(dpy, "_NET_WM_STATE_BELOW", False);

    XChangeProperty(
        dpy,
        win,
        state,
        XA_ATOM,
        32,
        PropModeReplace,
        (unsigned char *)&below,
        1);

    XMapWindow(dpy, win);
    XLowerWindow(dpy, win);

    GLXContext ctx =
        glXCreateContext(
            dpy,
            vi,
            nullptr,
            True);

    glXMakeCurrent(dpy, win, ctx);

    // ---------------------------
    // OpenGL Initialization
    // ---------------------------

    glViewport(0, 0, W, H);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)W / (float)H;

    glFrustum(
        -aspect,
         aspect,
        -1.0,
         1.0,
         1.0,
         100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    float angle = 0.0f;

    // ---------------------------
    // Main Loop
    // ---------------------------

    while (true)
    {
        while (XPending(dpy))
        {
            XEvent e;
            XNextEvent(dpy, &e);

            if (e.type == ConfigureNotify)
            {
                W = e.xconfigure.width;
                H = e.xconfigure.height;

                glViewport(0, 0, W, H);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

                aspect = (float)W / (float)H;

                glFrustum(
                    -aspect,
                     aspect,
                    -1.0,
                     1.0,
                     1.0,
                     100.0);

                glMatrixMode(GL_MODELVIEW);
            }
        }

        angle += 1.0f;

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, -5.0f);

        glRotatef(angle, 1.0f, 1.0f, 0.0f);

        glBegin(GL_QUADS);

        // Front
        glColor3f(1,0,0);
        glVertex3f(-1,-1, 1);
        glVertex3f( 1,-1, 1);
        glVertex3f( 1, 1, 1);
        glVertex3f(-1, 1, 1);

        // Back
        glColor3f(0,1,0);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1, 1,-1);
        glVertex3f( 1, 1,-1);
        glVertex3f( 1,-1,-1);

        // Left
        glColor3f(0,0,1);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1,-1, 1);
        glVertex3f(-1, 1, 1);
        glVertex3f(-1, 1,-1);

        // Right
        glColor3f(1,1,0);
        glVertex3f( 1,-1,-1);
        glVertex3f( 1, 1,-1);
        glVertex3f( 1, 1, 1);
        glVertex3f( 1,-1, 1);

        // Top
        glColor3f(1,0,1);
        glVertex3f(-1, 1,-1);
        glVertex3f(-1, 1, 1);
        glVertex3f( 1, 1, 1);
        glVertex3f( 1, 1,-1);

        // Bottom
        glColor3f(0,1,1);
        glVertex3f(-1,-1,-1);
        glVertex3f( 1,-1,-1);
        glVertex3f( 1,-1, 1);
        glVertex3f(-1,-1, 1);

        glEnd();

        glXSwapBuffers(dpy, win);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(33));
    }

    return 0;
}