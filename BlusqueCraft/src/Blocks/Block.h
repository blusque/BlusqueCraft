#pragma once

#include <array>
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
        std::vector<unsigned int> Indices { 0, 1, 2, /*\\*/ 2, 3, 0 };
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

        std::vector<unsigned int> operator+(unsigned int offset) const
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

        std::vector<unsigned int> GetOffsetPlaneIndices(int id, unsigned int offset) const
        {
            return Planes[id] + offset;
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
        Block(Vec3 pos, std::array<Vec2, 6> texOffset);
        virtual ~Block() = default;

        std::vector<Vertex> GetVertices() const;
        std::vector<unsigned int> GetIndices(unsigned int start) const;
    protected:
        Box m_Box;
    };
}

