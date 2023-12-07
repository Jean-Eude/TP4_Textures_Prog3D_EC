#include "Context.h"
// GLUT includes
#include <GL/glut.h>

void idle() {
    glutPostRedisplay();
}

void reshape(int w, int h) {
    Context::camera.resize(w, h);
    // refresh
    Context::refreshMatrices = true;
}


Camera Context::camera;
std::vector<Instance> Context::instances;
std::vector<Material*> Context::materials;
std::vector<Mesh*> Context::meshes;
bool Context::refreshMatrices = true;
bool Context::fullScreen = false;
unsigned int Context::skyboxVAO, Context::skyboxVBO;
unsigned int Context::skyboxTex = 0;
GLuint Context::m_skybox_program;

void clearContext() {
  for (Mesh* mesh: Context::meshes) {
    delete mesh;
  }
  for (Material* mat: Context::materials) {
    delete mat;
  }
  Context::meshes.clear();
  Context::materials.clear();
}

void Context::initSkybox() {
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &Context::skyboxVAO);
    glGenBuffers(1, &Context::skyboxVBO);
    glBindVertexArray(Context::skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, Context::skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Context::m_skybox_program = load_shaders("shaders/unlit/vertex_Skybox.glsl", "shaders/unlit/fragment_Skybox.glsl");

    std::vector<std::string> imagesHDR;
    imagesHDR.push_back("data/Skybox/Sky/px.png");
    imagesHDR.push_back("data/Skybox/Sky/nx.png");
    imagesHDR.push_back("data/Skybox/Sky/py.png");
    imagesHDR.push_back("data/Skybox/Sky/ny.png");
    imagesHDR.push_back("data/Skybox/Sky/pz.png");
    imagesHDR.push_back("data/Skybox/Sky/nz.png");

    Context::skyboxTex = loadCubemap(imagesHDR);

    // Utilisation de glUseProgram pour activer le programme avant de définir des uniformes
    glUseProgram(Context::m_skybox_program);
    glUniform1i(glGetUniformLocation(Context::m_skybox_program, "skybox"), 0);
}

void setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(Context::m_skybox_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void use() {
    glUseProgram(Context::m_skybox_program);
}

void setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(Context::m_skybox_program, name.c_str()), value);
}



void Context::drawSkybox() {
    // Pas besoin de rappeler glUseProgram ici, car il a déjà été appelé dans initSkybox.
    glDepthFunc(GL_LEQUAL);
    glUseProgram(Context::m_skybox_program);
    glm::mat4 view = glm::mat4(glm::mat3(Context::camera.getViewMatrix())); // remove translation from the view matrix
    setMat4("view", view);
    setMat4("projection", Context::camera.getProjectionMatrix());
    setMat4("model", glm::mat4(1.0f));

    // skybox cube
    glBindVertexArray(Context::skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}
