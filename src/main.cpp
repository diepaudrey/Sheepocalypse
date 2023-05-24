
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
        ctx.main_canvas_width();
        ctx.main_canvas_height();
        glClearColor(0.188235294f, 0.364705882f, 0.631372549f, 1.f);
        DragonGame.RenderFinal(ctx, dragonParam);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
