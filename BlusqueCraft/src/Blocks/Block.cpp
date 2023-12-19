#include "Block.h"

namespace BC
{
    Block::Block(Vec3 pos, const std::vector<Vec2>& texOffset)
        : m_Neighbor(std::vector(6, false))
    {
        m_Box += pos;
        for (int i = 0; i < 6; i++)
        {
            m_Box.SetPlaneTexOffset(i, texOffset[i]);
        }
    }

    std::vector<Vertex> Block::GetVertices() const
    {
        auto result = std::vector<Vertex>();
        for (int i = 0; i < 6; i++)
        {
            if (!m_Neighbor[i])
            {
                auto const s = result.size();
                result.resize(result.size() + 4);
                auto vertices = m_Box.Planes[i].Vertices;
                std::copy(vertices.begin(), vertices.end(), result.begin() + s);
                // for (int j = 0; j < 4; j++)
                // {
                //     result[i * 4 + j] = m_Box.Planes[i].Vertices[j];
                // }
            }
        }
        return result;
    }

    std::vector<unsigned> Block::GetIndices(unsigned start) const
    {
        auto result = std::vector<unsigned int>();
        auto count = 0;
        for (int i = 0; i < 6; i++)
        {
            if (!m_Neighbor[i])
            {
                auto const s = result.size();
                result.resize(result.size() + 6);
                auto indices = m_Box.GetOffsetPlaneIndices(i, start + count * 4);
                std::copy(indices.begin(), indices.end(), result.begin() + s);
                count++;
            }
        }
        return result;
    }

    void Block::SetNeighbor(int index)
    {
        m_Neighbor[index] = true;
    }
}
