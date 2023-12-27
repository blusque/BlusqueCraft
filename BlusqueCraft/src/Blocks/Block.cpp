#include "Block.h"

#include <iostream>

namespace BC
{
    Block::Block(iVec3 chunkPos, iVec3 chunkCoord, const std::vector<Vec2>& texOffset)
        : m_TexOffset(texOffset)
        , m_GlobalCoord(chunkCoord + chunkPos * 16)
        , m_ChunkCoord(chunkCoord)
        , m_Neighbor(std::vector(6, false)) {}

    std::vector<Vertex> Block::GetVertices() const
    {
        auto result = std::vector<Vertex>();
        auto box = Box();
        box += static_cast<Vec3>(m_GlobalCoord);
        for (int i = 0; i < 6; i++)
        {
            box.SetPlaneTexOffset(i, m_TexOffset[i]);
        }
        for (int i = 0; i < 6; i++)
        {
            if (!m_Neighbor[i])
            {
                auto const s = result.size();
                result.resize(result.size() + 4);
                auto vertices = box.Planes[i].Vertices;
                std::copy(vertices.begin(), vertices.end(), result.begin() + s);
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
                auto indices = Plane::GetOffsetPlaneIndices(start + count * 4);
                std::copy(indices.begin(), indices.end(), result.begin() + s);
                count++;
            }
        }
        return result;
    }

    void Block::SetNeighbor(int index, bool neighbor)
    {
        m_Neighbor[index] = neighbor;
    }
}
