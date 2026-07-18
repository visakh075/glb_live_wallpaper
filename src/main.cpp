#include "Window.h"
#include "Renderer.h"
// #include "GameOfLife.h"
#include "Boids.h"
#include "Input.h"

#include <chrono>

int main()
{
    WindowManager window;

    if (!window.create())
        return -1;

    Renderer renderer;

    if (!renderer.initialize())
        return -1;

    renderer.resize(
        window.width(),
        window.height());

    // GameOfLife effect;
    Boids effect;

    effect.initialize(renderer);

    effect.resize(
        window.width(),
        window.height());

    using clock = std::chrono::steady_clock;

    auto previous = clock::now();

    while (window.running())
    {
        auto current = clock::now();

        float dt =
            std::chrono::duration<float>(
                current - previous).count();

        previous = current;

        //------------------------------------------------
        // Process Window Events
        //------------------------------------------------

        window.pollEvents(effect);

        //------------------------------------------------
        // Update Effect
        //------------------------------------------------

        effect.update(dt);

        //------------------------------------------------
        // Render
        //------------------------------------------------

        renderer.beginFrame();

        effect.render(renderer);

        renderer.endFrame();

        window.swapBuffers();
    }

    effect.shutdown();

    renderer.shutdown();

    return 0;
}