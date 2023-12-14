#pragma once
#include <memory>

#include "VertexArrayLayout.h"

#define OFFSET(x) (reinterpret_cast<const void*>(x))

namespace BC
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void SetupLayout(const VertexArrayLayout& layout) const;
        
        void Bind() const;

    private:
        unsigned int m_Renderer;
    };

    using VertexArrayPtr = std::shared_ptr<VertexArray>;
    using VertexArrayUPtr = std::unique_ptr<VertexArray>;
}
