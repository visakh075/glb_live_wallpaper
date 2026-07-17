#include "Input.h"

#include <X11/keysym.h>

#include "Effect.h"
#include "Window.h"

void Input::processEvent(
    const XEvent& event,
    WindowManager& window,
    Effect& effect)
{
    switch(event.type)
    {
        case ConfigureNotify:
        {
            effect.resize(
                event.xconfigure.width,
                event.xconfigure.height);

            break;
        }

        case KeyPress:
        {
            KeySym key =
                XLookupKeysym(
                    const_cast<XKeyEvent*>(&event.xkey),
                    0);

            effect.keyPress(key);

            break;
        }

        case KeyRelease:
        {
            KeySym key =
                XLookupKeysym(
                    const_cast<XKeyEvent*>(&event.xkey),
                    0);

            effect.keyRelease(key);

            break;
        }

        case ButtonPress:
        {
            effect.mousePress(
                event.xbutton.button,
                event.xbutton.x,
                event.xbutton.y);

            break;
        }

        case ButtonRelease:
        {
            effect.mouseRelease(
                event.xbutton.button,
                event.xbutton.x,
                event.xbutton.y);

            break;
        }

        case MotionNotify:
        {
            effect.mouseMove(
                event.xmotion.x,
                event.xmotion.y);

            break;
        }

        default:
            break;
    }
}