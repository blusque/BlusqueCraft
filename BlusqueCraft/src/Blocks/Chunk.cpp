#include "Chunk.h"

#include <iostream>

namespace BC
{
    Chunk::Chunk(int CoordX, int CoordZ)
        : m_CoordX(CoordX)
        , m_CoordZ(CoordZ)
    {
        m_Neighbor.resize(6);
        m_Blocks.resize(X_SIZE * Y_SIZE * Z_SIZE);
    }

    std::vector<Vertex> Chunk::GetVertices() const
    {
        std::vector<Vertex> data;
        
        for (auto&& block : m_Blocks)
        {
            auto vertices = block->GetVertices();
            auto const dS = static_cast<int>(data.size());
            data.resize(data.size() + vertices.size());
            std::copy(vertices.begin(), vertices.end(), data.begin() + dS);
        }
        return data;
    }

    std::vector<unsigned> Chunk::GetIndices() const
    {
        std::vector<unsigned int> dataIndices;
        unsigned int indexOffset = 0;
        for (auto&& block : m_Blocks)
        {
            auto indices = block->GetIndices(indexOffset);
            auto const iS = static_cast<int>(dataIndices.size());
            dataIndices.resize(dataIndices.size() + indices.size());
            std::copy(indices.begin(), indices.end(), dataIndices.begin() + iS);
            indexOffset += indices.size() / 6 * 4;
        }
        return dataIndices;   
    }

    void Chunk::SetNeighbor(int index, const ChunkPtr& neighbor)
    {
        m_Neighbor[index] = std::weak_ptr(neighbor);
    }

    void Chunk::GenerateBlock()
    {
        for (int k = 0; k < Z_SIZE; k++)
        {
            for (int j = 0; j < Y_SIZE; j++)
            {
                for (int i = 0; i < X_SIZE; i++)
                {
                    auto const index = k * Layer + j * Line + i;
                    m_Blocks[index] = std::make_unique<GrassBlock>(iVec3(m_CoordX, 0, m_CoordZ),
                        iVec3(i, j, k));
                    auto const rear = index >= Layer ? index - Layer : -1;
                    auto const left = index % Line != 0 ? index - 1 : -1;
                    auto const beneath = index % Layer >= Line ? index - Line : -1;
                    if (rear >= 0)
                    {
                        if (m_Blocks[rear])
                        {
                            m_Blocks[rear]->SetNeighbor(0);
                            m_Blocks[index]->SetNeighbor(1);
                        }
                    }
                    else
                    {
                        if (auto const neighbor = m_Neighbor[1].lock())
                        {
                            auto const pos = Total + index - Layer;
                            if (auto const block = neighbor->at(pos))
                            {
                                block->SetNeighbor(0);
                                m_Blocks[index]->SetNeighbor(1);
                            }
                        }
                    }
                    if (left >= 0)
                    {
                        if (m_Blocks[left])
                        {
                            m_Blocks[left]->SetNeighbor(2);
                            m_Blocks[index]->SetNeighbor(3);
                        }
                    }
                    else
                    {
                        if (auto const neighbor = m_Neighbor[3].lock())
                        {
                            auto const pos = index + Line - 1;
                            if (auto const block = neighbor->at(pos))
                            {
                                block->SetNeighbor(2);
                                m_Blocks[index]->SetNeighbor(3);
                            }
                        }
                    }
                    if (beneath >= 0)
                    {
                        if (m_Blocks[beneath])
                        {
                            m_Blocks[beneath]->SetNeighbor(4);
                            m_Blocks[index]->SetNeighbor(5);
                        }
                    }
                    // else
                    // {
                    //     if (auto const neighbor = m_Neighbor[5].lock())
                    //     {
                    //         auto const pos = (index / Layer + 1) * Layer - indexTotal + index - Layer;
                    //         if (auto const block = neighbor->at(pos))
                    //         {
                    //             block->SetNeighbor(4);
                    //             m_Blocks[index]->SetNeighbor(5);
                    //         }
                    //     }
                    // }
                }
            }
        }
    }
}
