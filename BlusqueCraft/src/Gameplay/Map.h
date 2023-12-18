#pragma once
#include <vector>

namespace BC
{
    struct MapProps
    {
        int LargeSizeX { 2048 };
        int LargeSizeY { 2048 };
        int LargeSizeZ { 2048 };
        int SizeX { 512 };
        int SizeY { 512 };
        int SizeZ { 512 };
    };
    
    class Map
    {
    public:
        Map(const MapProps& props);
        ~Map() = default;

        std::vector<bool> CheckBoundary() const;

    private:
        struct MapData
        {
            int LargeSizeX;
            int LargeSizeY;
            int LargeSizeZ;
            int SizeX;
            int SizeY;
            int SizeZ;
        };
        MapData m_Data;
        std::vector<bool> m_Map;
        std::vector<bool> m_LoadedMap;
    };
}

