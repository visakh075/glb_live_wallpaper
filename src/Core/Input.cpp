#include "Core/Input.h"

#include <X11/keysym.h>

#include "Core/Wallpaper.h"
#include "Core/Window.h"
#include "Core/Event.h"
void Input::processEvent(
    const XEvent& event,
    WindowManager& window,
    Wallpaper& effect)
{
    switch(event.type)
    {
        case ConfigureNotify:
        {
            Event e;

            e.type = Event::Type::WindowResize;

            e.width = event.xconfigure.width;

            e.height = event.xconfigure.height;

            effect.onEvent(e);
            break;
        }


        case KeyPress:
        {
            KeySym key =
                XLookupKeysym(
                    const_cast<XKeyEvent*>(&event.xkey),
                    0);

            Event e;

            e.type = Event::Type::KeyboardPress;

            e.key = static_cast<uint32_t>(key);

            effect.onEvent(e);

            break;
        }
        

        case KeyRelease:
        {
            KeySym key =
                XLookupKeysym(
                    const_cast<XKeyEvent*>(&event.xkey),
                    0);

            Event e;

            e.type = Event::Type::KeyboardRelease;

            e.key = static_cast<uint32_t>(key);

            effect.onEvent(e);

            break;
        }

        case ButtonPress:
        {
            Event e;

            e.type = Event::Type::MouseButtonPress;

            e.button = event.xbutton.button;

            e.x = event.xbutton.x;

            e.y = event.xbutton.y;

            effect.onEvent(e);

            break;
        }

        case ButtonRelease:
        {
            Event e;

            e.type = Event::Type::MouseButtonRelease;

            e.button = event.xbutton.button;

            e.x = event.xbutton.x;

            e.y = event.xbutton.y;

            effect.onEvent(e);

            break;
        }

        case MotionNotify:
        {
            Event e;

            e.type = Event::Type::MouseMotion;

            e.x = event.xmotion.x;

            e.y = event.xmotion.y;

            effect.onEvent(e);

            break;
        }

        default:
            break;
    }
}