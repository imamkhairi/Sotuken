#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <opencv2/opencv.hpp>

#include "shaderClass.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLenum ttype;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);
	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);
	Texture(cv::Mat image, const char* texType, GLuint slot);


	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

};
#endif