#pragma once

#include <X11/Xlib.h>

class WindowManager;
class Effect;

class Input
{
public:

    static void processEvent(
        const XEvent& event,
        WindowManager& window,
        Effect& effect);
};