﻿#include "IndexBuffer.h"

#include "glad/gl.h"

namespace BC
{
    IndexBuffer::IndexBuffer(const unsigned* data, unsigned count, VBUsage usage)
        : Buffer<unsigned int>(count)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * m_Size, data, static_cast<GLenum>(usage));
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_Renderer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer);
    }
}
