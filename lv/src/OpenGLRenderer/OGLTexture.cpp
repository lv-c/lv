#include <lv/OpenGLRenderer/OGLTexture.hpp>
#include <GL/GLU.h>

namespace lv
{
	OGLTexture::OGLTexture()
	{
		// generate a texture
		glGenTextures(1, &texture_);

		// parameters
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);	// GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);	// GL_CLAMP_TO_EDGE

	}

	OGLTexture::~OGLTexture()
	{
		glDeleteTextures(1, &texture_);
	}
}