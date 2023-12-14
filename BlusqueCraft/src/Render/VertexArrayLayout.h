#pragma once

#include <vector>

#include "glad/gl.h"

namespace BC
{
    struct LayoutData
    {
        int Count { 0 };
        unsigned Type { 0 };
        unsigned char Normalized { 0 };

        static unsigned int GetSizeofType(unsigned int type)
        {
            if (type == GL_FLOAT)
            {
                return sizeof(float);
            }
            if (type == GL_UNSIGNED_INT)
            {
                return sizeof(unsigned int);
            }
            if (type == GL_UNSIGNED_BYTE)
            {
                return 1;
            }
            
            return 0;
        }
    };
    
    class VertexArrayLayout
    {
    public:
        VertexArrayLayout() = default;
        ~VertexArrayLayout() = default;

        template<typename T>
        void Add(int count, unsigned char normalized, int stride = 0)
        {
            __debugbreak();
        }

        template<>
        void Add<float>(int count, unsigned char normalized, int stride)
        {
            m_Layout.push_back({count, GL_FLOAT, normalized});
            if (stride == 0)
            {
                m_Stride += count * static_cast<int>(LayoutData::GetSizeofType(GL_FLOAT));
            }
            else
            {
                m_Stride = stride;
            }
        }

        template<>
        void Add<unsigned int>(int count, unsigned char normalized, int stride)
        {
            m_Layout.push_back({count, GL_FLOAT, normalized});
            if (stride == 0)
            {
                m_Stride += count * static_cast<int>(LayoutData::GetSizeofType(GL_UNSIGNED_INT));
            }
            else
            {
                m_Stride = stride;
            }
        }

        template<>
        void Add<unsigned char>(int count, unsigned char normalized, int stride)
        {
            m_Layout.push_back({count, GL_FLOAT, normalized});
            if (stride == 0)
            {
                m_Stride += count * static_cast<int>(LayoutData::GetSizeofType(GL_UNSIGNED_BYTE));
            }
            else
            {
                m_Stride = stride;
            }
        }

        auto GetLayout() const
        {
            return m_Layout;
        }

        auto GetStride() const
        {
            return m_Stride;
        }

    private:
        std::vector<LayoutData> m_Layout;
        int m_Stride;
    };
}
