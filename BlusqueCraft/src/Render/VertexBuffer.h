#pragma once
#include <memory>

#include "glad/gl.h"

namespace BC
{
    enum class VBUsage
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW,
        STREAM = GL_STREAM_DRAW
    };

    template <typename T>
    class Buffer
    {
    public:
        virtual ~Buffer() { glDeleteBuffers(1, &m_Renderer); }

        virtual void Bind() const = 0;

        virtual unsigned int GetID() const { return m_Renderer; }

        virtual int GetCount() const { return m_Count; }

        virtual int GetElementSize() const { return m_Size; }

    protected:
        Buffer(unsigned int count)
            : m_Renderer(0), m_Count(count), m_Size(sizeof(T))
        {
            glGenBuffers(1, &m_Renderer);
        }
        
        unsigned int m_Renderer;
        int m_Count;
        int m_Size;
    };

    template<typename T>
    class VertexBuffer: public Buffer<T>
    {
    public:
        VertexBuffer(const T* data, unsigned int count, VBUsage usage)
            : Buffer<T>(count)
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_Renderer);
            glBufferData(GL_ARRAY_BUFFER, this->m_Count * this->m_Size, data, static_cast<GLenum>(usage));
        }
        ~VertexBuffer() override
        {
            glDeleteBuffers(1, &this->m_Renderer);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Bind() const override
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_Renderer);
        }

        [[nodiscard]] bool BufferData(const T* data, unsigned int count, VBUsage usage) const
        {
            if (count != this->m_Count)
            {
                return false;   
            }
            Bind();
            glBufferData(GL_ARRAY_BUFFER, this->m_Count * this->m_Size, data, static_cast<GLenum>(usage));
            return true;
        }
    };

    template<typename T>
    using VertexBufferPtr = std::shared_ptr<VertexBuffer<T>>;
    template<typename T>
    using VertexBufferUPtr = std::unique_ptr<VertexBuffer<T>>;
}
