// Local includes
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>


#include "Context.h"

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
	m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	
	m_texture = loadTexture2DFromFilePath("data/Textures/rock_wall_10_diff_1k.jpg");	// Exercice 1

	// Exercice 2
	m_texture1 = loadTexture2DFromFilePath("data/Textures/Coast_Sand/coast_sand_rocks_02_diff_1k.jpg");
	//m_texture1 = loadTexture2DFromFilePath("data/Textures/Default_albedo.jpg");

	m_texture2 = loadTexture2DFromFilePath("data/Textures/Coast_Sand/coast_sand_rocks_02_nor_1k.png");
	//m_texture2 = loadTexture2DFromFilePath("data/Textures/Default_normal.jpg");

	// Exercice 3

	m_skybox = Context::skyboxTex;
	
	// Exercice 4
	m_roughness = loadTexture2DFromFilePath("data/Textures/Coast_Sand/coast_sand_rocks_02_rough_1k.png");
	m_ao = loadTexture2DFromFilePath("data/Textures/Coast_Sand/coast_sand_rocks_02_ao_1k.png");

	//m_roughness = loadTexture2DFromFilePath("data/Textures/Default_metalRoughness.jpg");
	//m_ao = loadTexture2DFromFilePath("data/Textures/Default_AO.jpg");
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind() {
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {

	/*
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	if (m_texture != -1) {
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(getUniform("colorTexture"), GL_TEXTURE0);
	}
	*/

	/*
	// Exercice 2
	if (m_texture1 != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture1);
		glUniform1i(getUniform("colorTexture1"), 0);
	} 

	if (m_texture2 != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture2);
		glUniform1i(getUniform("colorTexture2"), 1);
	} 

	GLint viewPosLocation = glGetUniformLocation(m_program, "viewPos");
	GLint lightPosLocation = glGetUniformLocation(m_program, "lightPos");

	glm::vec3 viewPosition = Context::camera.getPosition();
	glm::vec3 lightPosition = glm::vec3(-1.0f, 6.0f, 2.0f);

	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPosition));
	glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition));
	*/

	/*
	// Exercice 3 (Basique)
	if (m_skybox != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox);
		glUniform1i(getUniform("skybox"), 0);
	}
	
	GLint viewPosLocation = glGetUniformLocation(m_program, "viewPos");
	glm::vec3 viewPosition = Context::camera.getPosition();
	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPosition));
	*/

	/*
	// Exercice 3 (Avec Normal Mapping)
	if (m_skybox != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox);
		glUniform1i(getUniform("skybox"), 0);
	}

	if (m_texture1 != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture1);
		glUniform1i(getUniform("colorTexture1"), 1);
	} 

	if (m_texture2 != -1) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_texture2);
		glUniform1i(getUniform("colorTexture2"), 2);
	} 

	GLint viewPosLocation = glGetUniformLocation(m_program, "viewPos");
	GLint lightPosLocation = glGetUniformLocation(m_program, "lightPos");

	glm::vec3 viewPosition = Context::camera.getPosition();
	glm::vec3 lightPosition = glm::vec3(-1.0f, 6.0f, 2.0f);

	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPosition));
	glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition));
	*/

	
	// Exercice 4 (PBR)
	if (m_skybox != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox);
		glUniform1i(getUniform("skybox"), 0);
	}

	if (m_texture1 != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture1);
		glUniform1i(getUniform("colorTexture1"), 1);
	} 

	if (m_texture2 != -1) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_texture2);
		glUniform1i(getUniform("colorTexture2"), 2);
	} 

	if (m_roughness != -1) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_roughness);
		glUniform1i(getUniform("roughness"), 3);
	} 

	if (m_ao != -1) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_ao);
		glUniform1i(getUniform("ao"), 4);
	} 

	GLint viewPosLocation = glGetUniformLocation(m_program, "viewPos");
	GLint lightPosLocation = glGetUniformLocation(m_program, "lightPos");

	glm::vec3 viewPosition = Context::camera.getPosition();
	glm::vec3 lightPosition = glm::vec3(0., 0., 0.);

	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPosition));
	glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition));
	
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
