#include "Resources/Texture.h"

#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utils/Logger.h"


Texture::Texture(const std::string& path)
	: mTexturerID(0), mType(GL_TEXTURE_2D), mFilePath(path),
	mBuffer(nullptr), mWidth(0), mHeight(0), mBPP(0)
{

	// OpenGL and STBI use different coordinate systems
	// That being said some textures work flipped and unflipped
	// Documentation recommends flipping all textures
	stbi_set_flip_vertically_on_load(true);
	mBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBPP, 4);

	GLCall(glGenTextures(1, &mTexturerID));
	GLCall(glBindTexture(mType, mTexturerID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE))

	if (mBuffer)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(mBuffer);
	}
	else
	{
		std::cout << "\nError: Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
		__debugbreak();
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mTexturerID));
}

void Texture::Bind() const
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(mType, mTexturerID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(mType, 0));
}
