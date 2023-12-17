#pragma once

#include <array>

#include "Utils/Math.h"

namespace BC
{
    struct Vertex
    {
        Vec3 PointCoords { 0.f, 0.f, 0.f };
        Vec2 TexCoords { 0.f, 0.f };
        float TexIndex { 0.f };

        Vertex(Vec3 point, Vec2 texCoord, float id)
            : PointCoords(point), TexCoords(texCoord), TexIndex(id) {}
    };

    struct Box
    {
        std::array<Vertex, 24> Vertices {
            Vertex(Vec3(-0.5f, -0.5f,  0.5f), Vec2(0.0f, 0.0f), 0.0f),
            {{ 0.5f, -0.5f,  0.5f},{1.0f, 0.0f}, 0.0f},
            {{ 0.5f,  0.5f,  0.5f},{1.0f, 1.0f}, 0.0f},
            {{-0.5f,  0.5f,  0.5f},{0.0f, 1.0f}, 0.0f},
            
            {{ 0.5f, -0.5f, -0.5f},{0.0f, 0.0f}, 0.0f},
            {{-0.5f, -0.5f, -0.5f},{1.0f, 0.0f}, 0.0f},
            {{-0.5f,  0.5f, -0.5f},{1.0f, 1.0f}, 0.0f},
            {{ 0.5f,  0.5f, -0.5f},{0.0f, 1.0f}, 0.0f},

            {{ 0.5f, -0.5f,  0.5f},{0.0f, 0.0f}, 0.0f},
            {{ 0.5f, -0.5f, -0.5f},{1.0f, 0.0f}, 0.0f},
            {{ 0.5f,  0.5f, -0.5f},{1.0f, 1.0f}, 0.0f},
            {{ 0.5f,  0.5f,  0.5f},{0.0f, 1.0f}, 0.0f},
            
            {{-0.5f, -0.5f, -0.5f},{0.0f, 0.0f}, 0.0f},
            {{-0.5f, -0.5f,  0.5f},{1.0f, 0.0f}, 0.0f},
            {{-0.5f,  0.5f,  0.5f},{1.0f, 1.0f}, 0.0f},
            {{-0.5f,  0.5f, -0.5f},{0.0f, 1.0f}, 0.0f},
            
            {{-0.5f,  0.5f,  0.5f},{0.0f, 0.0f}, 2.0f},
            {{ 0.5f,  0.5f,  0.5f},{1.0f, 0.0f}, 2.0f},
            {{ 0.5f,  0.5f, -0.5f},{1.0f, 1.0f}, 2.0f},
            {{-0.5f,  0.5f, -0.5f},{0.0f, 1.0f}, 2.0f},
            
            {{-0.5f, -0.5f, -0.5f},{0.0f, 0.0f}, 1.0f},
            {{ 0.5f, -0.5f, -0.5f},{1.0f, 0.0f}, 1.0f},
            {{ 0.5f, -0.5f,  0.5f},{1.0f, 1.0f}, 1.0f},
            {{-0.5f, -0.5f,  0.5f},{0.0f, 1.0f}, 1.0f}
        };

        Box operator+(Vec3 position) const
        {
            Box result;
            for (int i = 0; i < 24; i++)
            {
                result.Vertices[i] = Vertices[i];
                result.Vertices[i].PointCoords = Vertices[i].PointCoords + position;
            }
            return result;
        }

        void operator+=(Vec3 position)
        {
            for (int i = 0; i < 24; i++)
            {
                Vertices[i].PointCoords += position;
            }
        }
    };

    inline Box operator+(Vec3 position, const Box& box)
    {
        Box result;
        for (int i = 0; i < 24; i++)
        {
            result.Vertices[i] = box.Vertices[i];
            result.Vertices[i].PointCoords = box.Vertices[i].PointCoords + position;
        }
        return result;
    }

    enum class BlockType
    {
        
    };
    
    class Block
    {
    public:
        virtual ~Block() = default;

    protected:
        Block(int CoordX, int CoordY, int CoordZ);

        int m_CoordX;
        int m_CoordY;
        int m_CoordZ;
    };
}

