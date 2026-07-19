#pragma once

#include <X11/Xlib.h>

class WindowManager;
class Wallpaper;

class Input
{
public:

    static void processEvent(
        const XEvent& event,
        WindowManager& window,
        Wallpaper& effect);
};