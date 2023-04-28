
#include <stddef.h>
#include "Game.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "Dragons"}};
    ctx.maximize_window();

    BoidsParameters dragonParam;
    // LightParams     lighP;
    // lighP.light          = glm::vec3(0.f, 50.f, 0.f);
    // lighP.lightIntensity = glm::vec3(100.f, 100.f, 100.f);
    // lighP.Ka             = glm::vec3(0.05, 0.05, 0.05);
    // lighP.Kd             = glm::vec3(1.0, 1.0, 1.0);
    // lighP.Ks             = glm::vec3(1.0, 1.0, 1.0);
    // lighP.shininess      = 0.5f;
    Game DragonGame(ctx, dragonParam);

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
