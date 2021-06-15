#pragma once
#include <stdint.h>

#include <glad/glad.h>
#include <stb_image.h>

#include "XUtils.h"

class Texture{
public:
	unsigned int ID;
	int m_width;
	int m_height;
    unsigned char * m_data;

	Texture(const char*path) {
		auto path_ = XUtils::GetRealPath(path, "image/");
		// assert res exsit

		// load and create a texture 
		// -------------------------
		ID = 0;
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		m_data = stbi_load(path_.c_str(), &width, &height, &nrChannels, 0);
		if (m_data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
			// glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		// stbi_image_free(data);

		ID = texture;
		m_width = width;
		m_height = height;
	}

    uint8_t & data(int i, int k, int m) {
        return m_data[m_width*3*i + 3*k + m];
    }

    void Clear(uint8_t r, uint8_t g, uint8_t b) {
        uint8_t colors[3] = { r,g,b };
        for (int i = 0; i < m_height; i++) {
            for (int k = 0; k < m_width; k++) {
                for (int m = 0; m < 3; m++) {
                    data(i, k, m) = colors[m];
                    // m_data[m_width*i + k] =
                }
            }
        }
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
    }

	void use(int idx = 0) {
		GLenum pos = GL_TEXTURE0 + idx;
		glActiveTexture(pos);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

};