#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Utils/Math.h"

namespace BC
{
    struct Vertex
    {
        Vec3 PointCoords { 0.f, 0.f, 0.f };
        Vec2 TexCoords { 0.f, 0.f };
        
        Vertex() = default;
        Vertex(Vec3 point, Vec2 texCoord)
            : PointCoords(point), TexCoords(texCoord) {}
    };

    struct Plane
    {
        std::vector<Vertex> Vertices;
        inline static std::vector<unsigned int> Indices { 0, 1, 2, /*\\*/ 2, 3, 0 };
        Plane() { Vertices.resize(4); }
        Plane(const std::vector<Vertex>& vertices) : Vertices(vertices) {}
        Plane operator+(Vec3 position) const
        {
            auto result = Plane();
            for (int i = 0; i < 4; i++)
            {
                result.Vertices[i] = Vertices[i];
                result.Vertices[i].PointCoords = Vertices[i].PointCoords + position;
            }
            return result;
        }

        Plane operator+(Vec2 offset) const
        {
            auto result = Plane();
            for (int i = 0; i < 4; i++)
            {
                result.Vertices[i] = Vertices[i];
                result.Vertices[i].TexCoords = Vertices[i].TexCoords + offset;
            }
            return result;
        }

        void operator+=(Vec3 position)
        {
            for (int i = 0; i < 4; i++)
            {
                Vertices[i].PointCoords += position;
            }
        }

        void operator+=(Vec2 offset)
        {
            for (int i = 0; i < 4; i++)
            {
                Vertices[i].TexCoords += offset;
            }
        }

        static std::vector<unsigned int> GetOffsetPlaneIndices(unsigned int offset)
        {
            auto result = Indices;
            for (auto& index : result)
            {
                index += offset;
            }
            return result;
        }
    };

    inline Plane operator+(Vec3 position, const Plane& plane)
    {
        return plane + position;
    }

    inline Plane operator+(Vec2 offset, const Plane& plane)
    {
        return plane + offset;
    }

    struct Box
    {
        std::vector<Plane> Planes {
            Plane({
                    Vertex({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}),
                    Vertex({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}),
                    Vertex({ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}),
                    Vertex({-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f})
            }),

            Plane({
                    Vertex({ 0.5f, -0.5f, -0.5f},{0.0f, 0.0f}),
                    Vertex({-0.5f, -0.5f, -0.5f},{1.0f, 0.0f}),
                    Vertex({-0.5f,  0.5f, -0.5f},{1.0f, 1.0f}),
                    Vertex({ 0.5f,  0.5f, -0.5f},{0.0f, 1.0f})
            }),
            
            Plane({
                    Vertex({ 0.5f, -0.5f,  0.5f},{0.0f, 0.0f}),
                    Vertex({ 0.5f, -0.5f, -0.5f},{1.0f, 0.0f}),
                    Vertex({ 0.5f,  0.5f, -0.5f},{1.0f, 1.0f}),
                    Vertex({ 0.5f,  0.5f,  0.5f},{0.0f, 1.0f})
            }),
            
            Plane({
                    Vertex({-0.5f, -0.5f, -0.5f},{0.0f, 0.0f}),
                    Vertex({-0.5f, -0.5f,  0.5f},{1.0f, 0.0f}),
                    Vertex({-0.5f,  0.5f,  0.5f},{1.0f, 1.0f}),
                    Vertex({-0.5f,  0.5f, -0.5f},{0.0f, 1.0f})
            }),
            
            Plane({
                    Vertex({-0.5f,  0.5f,  0.5f},{0.0f, 0.0f}),
                    Vertex({ 0.5f,  0.5f,  0.5f},{1.0f, 0.0f}),
                    Vertex({ 0.5f,  0.5f, -0.5f},{1.0f, 1.0f}),
                    Vertex({-0.5f,  0.5f, -0.5f},{0.0f, 1.0f})
            }),
            
            Plane({
                    Vertex({-0.5f, -0.5f, -0.5f},{0.0f, 0.0f}),
                    Vertex({ 0.5f, -0.5f, -0.5f},{1.0f, 0.0f}),
                    Vertex({ 0.5f, -0.5f,  0.5f},{1.0f, 1.0f}),
                    Vertex({-0.5f, -0.5f,  0.5f},{0.0f, 1.0f})
            })
        };

        static std::unique_ptr<Box> s_Box;

        static Box* Get() { return s_Box.get(); }

        Box operator+(Vec3 position) const
        {
            Box result;
            for (int i = 0; i < 6; i++)
            {
                result.Planes[i] = Planes[i];
                result.Planes[i] = Planes[i] + position;
            }
            return result;
        }

        Box operator+(Vec2 offset) const
        {
            Box result;
            for (int i = 0; i < 6; i++)
            {
                result.Planes[i] = Planes[i];
                result.Planes[i] = Planes[i] + offset;
            }
            return result;
        }

        void SetPlaneTexOffset(int id, Vec2 offset)
        {
            Planes[id] += offset;
        }

        void operator+=(Vec3 position)
        {
            for (int i = 0; i < 6; i++)
            {
                Planes[i] += position;
            }
        }

        void operator+=(Vec2 offset)
        {
            for (int i = 0; i < 6; i++)
            {
                Planes[i] += offset;
            }
        }
    };

    using BoxUPtr = std::unique_ptr<Box>;

    inline Box operator+(Vec3 position, const Box& box)
    {
        return box + position;
    }

    inline Box operator+(Vec2 offset, const Box& box)
    {
        return box + offset;
    }

    enum class BlockType
    {
        
    };
    
    class Block
    {
    public:
        Block(iVec3 chunkPos, iVec3 chunkCoord, const std::vector<Vec2>& texOffset);
        virtual ~Block() = default;

        std::vector<Vertex> GetVertices(int plane) const;
        static std::vector<unsigned int> GetIndices(unsigned int start);
        int CountVisiblePlane() const { return m_NumVisible; }
        bool BlockVisible() const { return m_NumVisible != 0; }
        bool PlaneVisible(int plane) const { return !m_Neighbor[plane]; }
        void SetNeighbor(int index, bool neighbor = true);
        iVec3 GetGlobalCoord() const { return m_GlobalCoord; }
        int GetGlobalCoordX() const { return GetGlobalCoord().x; }
        int GetGlobalCoordY() const { return GetGlobalCoord().y; }
        int GetGlobalCoordZ() const { return GetGlobalCoord().z; }

        iVec3 GetChunkCoord() const { return m_ChunkCoord; }
        int GetChunkCoordX() const { return GetChunkCoord().x; }
        int GetChunkCoordY() const { return GetChunkCoord().y; }
        int GetChunkCoordZ() const { return GetChunkCoord().z; }
        
    protected:
        std::vector<Vec2> m_TexOffset;
        iVec3 m_GlobalCoord;
        iVec3 m_ChunkCoord;
        std::vector<bool> m_Neighbor;
        int m_NumVisible { 6 };
    };

    using BlockPtr = std::shared_ptr<Block>;
    using BlockUPtr = std::unique_ptr<Block>;

    class GrassBlock : public Block
    {
    public:
        GrassBlock(iVec3 chunkPos, iVec3 chunkCoord)
            : Block(chunkPos, chunkCoord, std::vector<Vec2>{
                {1.f, 11.f},
                {1.f, 11.f},
                {1.f, 11.f},
                {1.f, 11.f},
                {2.f, 11.f},
                {2.f, 11.f}
            }) {}
        ~GrassBlock() override = default;
    };
}

