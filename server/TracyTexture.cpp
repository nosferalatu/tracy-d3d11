#include <gl/gl3w.h>

#include "TracyTexture.hpp"

namespace tracy
{

void* MakeTexture()
{
    GLuint tex;
    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    return (void*)(intptr_t)tex;
}

void FreeTexture( void* _tex )
{
    auto tex = (GLuint)(intptr_t)_tex;
    glDeleteTextures( 1, &tex );
}

void UpdateTexture( void* _tex, const char* data, int w, int h )
{
    auto tex = (GLuint)(intptr_t)_tex;
    glBindTexture( GL_TEXTURE_2D, tex );
    glCompressedTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB8_ETC2, w, h, 0, w * h / 2, data );
}

}
