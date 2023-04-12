// #include "Boid.hpp"

// void Boid::drawBoid(const p6::Shader& shader, glm::mat4 viewMatrix, p6::Context& ctx, GLuint uMVPMatrix, GLuint uMVMatrix, GLuint uNormalMatrix)
// {
//     // glm::mat4 viewMatrix   = camera.getViewMatrix();
//     glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
//     glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
//     glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

//     glimac::bind_default_shader();
//     shader.use();

//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;
//     glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

//     glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

//     glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

//     glBindVertexArray(m_vao);

//     glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

//     glBindVertexArray(0);
// }