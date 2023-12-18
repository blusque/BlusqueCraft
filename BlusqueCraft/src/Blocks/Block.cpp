#include "Block.h"

namespace BC
{
    Block::Block(Vec3 pos, std::array<Vec2, 6> texOffset)
    {
        m_Box += pos;
        for (int i = 0; i < 6; i++)
        {
            m_Box.SetPlaneTexOffset(i, texOffset[i]);
        }
    }

    std::vector<Vertex> Block::GetVertices() const
    {
        std::vector<Vertex> result;
        result.resize(24);
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result[i * 4 + j] = m_Box.Planes[i].Vertices[j];
            }
        }
        return result;
    }

    std::vector<unsigned> Block::GetIndices(unsigned start) const
    {
        auto result = std::vector<unsigned int>(36);
        for (int i = 0; i < 6; i++)
        {
            auto indices = m_Box.GetOffsetPlaneIndices(i, start + i * 4);
            std::copy(indices.begin(), indices.end(), result.begin() + i * 6);
        }
        return result;
    }
}
