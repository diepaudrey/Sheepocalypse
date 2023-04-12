

#include "../classes/Boid.hpp"
#include "cstddef"
#include "glimac/Freefly.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

void mouseHandler(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& rotationStrength)
{
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ctx.mouse_moved = [&](p6::MouseMove data) {
        (camera).rotateLeft(data.delta.x * rotationStrength);
        (camera).rotateUp(-data.delta.y * rotationStrength);
    };
}

void keyboardHandler(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& mvtStrength)
{
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        camera.moveFront(ctx.delta_time() * mvtStrength);
    };
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        camera.moveFront(-ctx.delta_time() * mvtStrength);
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        camera.moveLeft(ctx.delta_time() * mvtStrength);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        camera.moveLeft(-ctx.delta_time() * mvtStrength);
    }
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*Camera*/
    // glimac::TrackballCamera camera;
    glimac::FreeflyCamera camera;
    float                 movementStrength = 5.f;
    float                 rotationStrength = 900.f;

    mouseHandler(ctx, camera, rotationStrength);

    // /*VBO*/
    GLuint vbo;
    // std::vector<GLuint> VBOs;
    //  glGenBuffers(1, &vbo);
    //  glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);

    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // /*VAO*/
    GLuint vao;
    // std::vector<GLuint> VAOs;
    //  glGenVertexArrays(1, &vao);
    //  glBindVertexArray(vao);

    // static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    // static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
    // static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    // glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    // glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    // glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)nullptr);

    // glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));

    // glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glBindVertexArray(0);

    /*Loading Shader*/
    const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    /*Location uniform variables*/
    GLint uMVPMatrix = glGetUniformLocation(shader.id(), "uMVPMatrix");

    GLint uMVMatrix = glGetUniformLocation(shader.id(), "uMVMatrix");

    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glm::vec3 pos   = {p6::random::number(0, 1), p6::random::number(0, 1), p6::random::number(0, 1)};
    glm::vec3 speed = pos + (p6::random::number(0, 1), p6::random::number(0, 1), p6::random::number(0, 1));
    Boid      cone(vertices, vbo, vao, pos, speed);
    // cone.initializeBoid();

    glEnable(GL_DEPTH_TEST);

    mouseHandler(ctx, camera, rotationStrength);
    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*Events*/
        keyboardHandler(ctx, camera, movementStrength);

        cone.drawBoid(shader, camera.getViewMatrix(), ctx, uMVPMatrix, uMVMatrix, uNormalMatrix);
        cone.updatePosition(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);
}