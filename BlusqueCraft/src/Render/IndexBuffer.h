#pragma once
#include "VertexBuffer.h"

namespace BC
{
    class IndexBuffer: public Buffer<unsigned int>
    {
    public:
        IndexBuffer(const unsigned int* data, unsigned int count, VBUsage usage);
        ~IndexBuffer() override;

        void Bind() const override;

        [[nodiscard]] bool BufferData(const unsigned int* data, unsigned int count, VBUsage usage) const
        {
            if (count != m_Count)
            {
                return false;
            }
            Bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * m_Size, data, static_cast<GLenum>(usage));
            return true;
        }
    };

    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
    using IndexBufferUPtr = std::unique_ptr<IndexBuffer>;
}
