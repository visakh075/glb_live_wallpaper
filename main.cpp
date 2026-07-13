#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <chrono>
#include <thread>
#include <vector>

#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <X11/keysym.h>

const int CELL_SIZE = 3;

int GRID_W = 0;
int GRID_H = 0;

std::vector<uint8_t> grid;
std::vector<uint8_t> nextGrid;

void updateLife();
void updateLife()
{
    for (int y = 0; y < GRID_H; y++)
    {
        for (int x = 0; x < GRID_W; x++)
        {
            int neighbours = 0;

            for (int yy = -1; yy <= 1; yy++)
            {
                for (int xx = -1; xx <= 1; xx++)
                {
                    if (xx == 0 && yy == 0)
                        continue;

                    int nx = (x + xx + GRID_W) % GRID_W;
                    int ny = (y + yy + GRID_H) % GRID_H;

                    neighbours +=
                        grid[ny * GRID_W + nx];
                }
            }

            bool alive =
                grid[y * GRID_W + x];

            if (alive)
            {
                nextGrid[y * GRID_W + x] =
                    (neighbours == 2 ||
                     neighbours == 3);
            }
            else
            {
                nextGrid[y * GRID_W + x] =
                    (neighbours == 3);
            }
        }
    }

    grid.swap(nextGrid);
}

void randomizeGrid()
{
    srand(time(nullptr));

    for (auto &c : grid)
        c = rand() % 5 == 0;
}

void resizeGrid(int W, int H)
{
    GRID_W = (W + CELL_SIZE - 1) / CELL_SIZE;
    GRID_H = (H + CELL_SIZE - 1) / CELL_SIZE;

    grid.resize(GRID_W * GRID_H);
    nextGrid.resize(GRID_W * GRID_H);

    randomizeGrid();
}

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
    swa.event_mask =
    ExposureMask |
    StructureNotifyMask |
    KeyPressMask;

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

    resizeGrid(W,H);
    // float cell =
    //     std::min(
    //         (float)W / GRID_W,
    //         (float)H / GRID_H);

    float cell = CELL_SIZE;

    float offsetX =
        (W - GRID_W * cell) * 0.5f;

    float offsetY =
        (H - GRID_H * cell) * 0.5f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glOrtho(
        0,
        W,
        H,
        0,
        -1,
        1);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);



    while (true)
    {
    while (XPending(dpy))
    {
        XEvent e;
        XNextEvent(dpy, &e);

        if (e.type == KeyPress)
        {
            KeySym key =
                XLookupKeysym(&e.xkey,0);

            if(key == XK_r)
            {
                for(auto &c : grid)
                    c = rand()%5==0;
            }
        }

        if (e.type == ConfigureNotify)
        {
            W = e.xconfigure.width;
            H = e.xconfigure.height;

            glViewport(0, 0, W, H);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glOrtho(
                0.0,
                GRID_W,
                GRID_H,
                0.0,
                -1.0,
                1.0);

            glMatrixMode(GL_MODELVIEW);
        }
    }

    //-------------------------------------------------
    // Update simulation
    //-------------------------------------------------

    updateLife();


    int alive = 0;

    for(auto c : grid)
        alive += c;

    if(alive < 25)
    {
        for(auto &c : grid)
            c = rand()%5==0;
    }
        //-------------------------------------------------
    // Draw
    //-------------------------------------------------

    glClearColor(0.05f,0.05f,0.08f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cell = CELL_SIZE;

    float offsetX =
        (W - GRID_W * cell) * 0.5f;

    float offsetY =
        (H - GRID_H * cell) * 0.5f;

    glBegin(GL_QUADS);

    for(int y=0;y<GRID_H;y++)
    {
        for(int x=0;x<GRID_W;x++)
        {
            if(!grid[y*GRID_W+x])
                continue;

            glColor3f(
                0.2f,
                0.55f,
                1.0f);

            float px = offsetX + x * cell;
            float py = offsetY + y * cell;

            glVertex2f(px, py);
            glVertex2f(px + cell, py);
            glVertex2f(px + cell, py + cell);
            glVertex2f(px, py + cell);
        }
    }

    glEnd();

    glXSwapBuffers(dpy,win);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(33));
    }

    return 0;
}