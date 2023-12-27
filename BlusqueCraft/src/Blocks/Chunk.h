#pragma once
#include <vector>

#include "Block.h"

#define X_SIZE 16
#define Y_SIZE 16
#define Z_SIZE 16

namespace BC
{
    class Chunk
    {
    private:
        using ChunkPtr = std::shared_ptr<Chunk>;
        using ChunkUPtr = std::unique_ptr<Chunk>;
        using ChunkWPtr = std::weak_ptr<Chunk>;
    public:
        Chunk(int CoordX = 0, int CoordZ = 0);
        virtual ~Chunk() = default;

        std::vector<Vertex> GetVertices() const;
        std::vector<unsigned int> GetIndices() const;

        int GetCoordX() const { return m_CoordX; }
        int GetCoordZ() const { return m_CoordZ; }
        Block* at(int pos) const { return m_Blocks[pos].get(); }
        Block* at(int x, int y, int z) const { return at({ x, y, z }); }
        Block* at(iVec3 coord) const { return m_Blocks[coord.z * Layer + coord.y * Line + coord.x ].get(); }
        void SetNeighbor(int index, const ChunkPtr& neighbor);
        void GenerateBlock();

    protected:
        int m_CoordX;
        int m_CoordZ;

        inline static int Line = X_SIZE;
        inline static int Layer = Y_SIZE * X_SIZE;
        inline static int Total = Z_SIZE * Y_SIZE * X_SIZE;

        std::vector<BlockUPtr> m_Blocks;
        std::vector<ChunkWPtr> m_Neighbor; 
    };

    using ChunkPtr = std::shared_ptr<Chunk>;
    using ChunkUPtr = std::unique_ptr<Chunk>;
}

