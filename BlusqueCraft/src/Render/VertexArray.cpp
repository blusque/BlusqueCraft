#include "VertexArray.h"

namespace BC
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_Renderer);
        glBindVertexArray(m_Renderer);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_Renderer);
        glBindVertexArray(0);
    }

    void VertexArray::SetupLayout(const VertexArrayLayout& layout) const
    {
        Bind();
        auto i = 0;
        auto offset = 0;
        for (auto&& l : layout.GetLayout())
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, l.Count, l.Type, l.Normalized, layout.GetStride(), OFFSET(offset));
            offset += l.Count * static_cast<int>(LayoutData::GetSizeofType(l.Type));
            i++;
        }
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_Renderer);
    }
}
