#pragma once

namespace BC
{
    class Chunk
    {
    public:
        Chunk(int CoordX, int CoordZ);
        ~Chunk();

    private:
        int CoordX;
        int CoordZ;
    };
}

