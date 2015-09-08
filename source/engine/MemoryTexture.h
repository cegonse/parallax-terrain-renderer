#ifndef _MEMORYTEXTURE_H_
#define _MEMORYTEXTURE_H_

#include <GL\glew.h>
#include <cstdlib>
#include <string>

class MemoryTexture
{
	public:
		MemoryTexture();
		MemoryTexture(int width, int height);
		~MemoryTexture();

		int GetWidth() const;
		int GetHeight() const;

		void Set(GLubyte* data);
		GLubyte* Get() const;

	private:
		GLubyte* _buffer;
		int _width;
		int _height;

		void Allocate();
};

#endif // _MEMORYTEXTURE_H_