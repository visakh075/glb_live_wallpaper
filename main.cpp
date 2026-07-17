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

const int CELL_SIZE = 4;

int GRID_W = 0;
int GRID_H = 0;

std::vector<uint8_t> grid;
std::vector<uint8_t> nextGrid;
std::vector<float> alpha;

bool dragging = false;

int dragStartX = 0;
int dragStartY = 0;

int dragEndX = 0;
int dragEndY = 0;

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

struct Cell
{
    bool alive = false;
    bool nextAlive = false;

    float alpha = 0.0f;
    float scale = 0.0f;

    uint8_t age = 0;
};

class GameOfLife
{
public:

    void resize(int w,int h);

    void randomize();

    void update();

    void draw();

    void clear();

    void fillRectangle(
        int x0,
        int y0,
        int x1,
        int y1);

private:

    int width;
    int height;

    std::vector<Cell> cells;
};


void randomizeGrid()
{
    srand(time(nullptr));

    for (int i = 0; i < GRID_W * GRID_H; i++)
    {
        grid[i] = rand() % 5 == 0;

        alpha[i] = grid[i] ? 1.0f : 0.0f;
    }

}

void resizeGrid(int W, int H)
{
    GRID_W = (W + CELL_SIZE - 1) / CELL_SIZE;
    GRID_H = (H + CELL_SIZE - 1) / CELL_SIZE;

    grid.resize(GRID_W * GRID_H);
    nextGrid.resize(GRID_W * GRID_H);
    alpha.resize(GRID_W * GRID_H);

    for (auto &a : alpha)
        a = 0.0f;
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

inline int mouseToGridX(int x)
{
    return x / CELL_SIZE;
}

inline int mouseToGridY(int y)
{
    return y / CELL_SIZE;
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
        KeyPressMask |
        ButtonPressMask |
        ButtonReleaseMask |
        PointerMotionMask;

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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
            
            if (e.type == ButtonPress)
            {
                if (e.xbutton.button == Button3)
                {
                    dragging = true;

                    dragStartX = mouseToGridX(e.xbutton.x);
                    dragStartY = mouseToGridY(e.xbutton.y);

                    dragEndX = dragStartX;
                    dragEndY = dragStartY;
                }
            }

            if (e.type == MotionNotify && dragging)
            {
                dragEndX = mouseToGridX(e.xmotion.x);
                dragEndY = mouseToGridY(e.xmotion.y);
            }

            if (e.type == ButtonRelease)
            {
                if (e.xbutton.button == Button3)
                {
                    dragging = false;

                    int x0 = std::min(dragStartX, dragEndX);
                    int x1 = std::max(dragStartX, dragEndX);

                    int y0 = std::min(dragStartY, dragEndY);
                    int y1 = std::max(dragStartY, dragEndY);

                    for (int y = y0; y <= y1; y++)
                    {
                        for (int x = x0; x <= x1; x++)
                        {
                            if (x < 0 || y < 0 ||
                                x >= GRID_W || y >= GRID_H)
                                continue;

                            grid[y * GRID_W + x] =
                                rand() % 2;
                        }
                    }
                }
            }

            if (e.type == ConfigureNotify)
            {
                W = e.xconfigure.width;
                H = e.xconfigure.height;

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

    for(int i=0;i<GRID_W*GRID_H;i++)
    {
        if(grid[i])
        {
            alpha[i] += 0.15f;

            if(alpha[i] > 1.0f)
                alpha[i] = 1.0f;
        }
        else
        {
            alpha[i] -= 0.05f;

            if(alpha[i] < 0.0f)
                alpha[i] = 0.0f;
        }
    }

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

    if (dragging)
        {
            glColor3f(1.0f,1.0f,1.0f);

            glBegin(GL_LINE_LOOP);

            float x0 = dragStartX * CELL_SIZE;
            float y0 = dragStartY * CELL_SIZE;

            float x1 = dragEndX * CELL_SIZE;
            float y1 = dragEndY * CELL_SIZE;

            glVertex2f(x0,y0);
            glVertex2f(x1,y0);
            glVertex2f(x1,y1);
            glVertex2f(x0,y1);

            glEnd();
        }

    glBegin(GL_QUADS);


    for(int y=0;y<GRID_H;y++)
    {
        for(int x=0;x<GRID_W;x++)
        {
            if(!grid[y*GRID_W+x])
                continue;
            int index = y * GRID_W + x;

            if(alpha[index] <= 0.01f)
                continue;

            glColor4f(
                0.2f,
                0.55f,
                1.0f,
                alpha[index]);
                
            // glColor3f(
            //     0.2f,
            //     0.55f,
            //     1.0f);

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