#pragma once
#include "VertexBuffer.h"

namespace BC
{
    class IndexBuffer: public Buffer<unsigned int>
    {
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);

        ~IndexBuffer() override;

        void Bind() const override;
    };

    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
    using IndexBufferUPtr = std::unique_ptr<IndexBuffer>;
}
