
#include <stddef.h>
#include "Game.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "Dragons"}};
    ctx.maximize_window();

    BoidsParameters dragonParam;
    Game            DragonGame(ctx, dragonParam);

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*Events*/
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DragonGame.Render(ctx, dragonParam);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
