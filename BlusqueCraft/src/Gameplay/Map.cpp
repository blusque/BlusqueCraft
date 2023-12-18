#include "Map.h"

namespace BC
{
    Map::Map(const MapProps& props)
    {
        m_Data.SizeX = props.SizeX;
        m_Data.SizeY = props.SizeY;
        m_Data.SizeZ = props.SizeZ;
        m_Data.LargeSizeX = props.LargeSizeX;
        m_Data.LargeSizeY = props.LargeSizeY;
        m_Data.LargeSizeZ = props.LargeSizeZ;

        m_Map.resize(m_Data.SizeX * m_Data.SizeY * m_Data.SizeZ);
        m_LoadedMap.resize(m_Data.SizeX * m_Data.SizeY * m_Data.SizeZ);
    }

    std::vector<bool> Map::CheckBoundary() const
    {
        int const bSizeX = m_Data.SizeX - 1;
        int const bSizeY = m_Data.SizeY - 1;
        int const bSizeZ = m_Data.SizeZ - 1;
        auto result = std::vector(3 * bSizeX * bSizeY * bSizeZ, false);
        for (int i = 1; i < m_Data.SizeX - 1; i++)
        {
            for (int j = 1; j < m_Data.SizeY - 1; j++)
            {
                for (int k = 1; k < m_Data.SizeZ - 1; k++)
                {
                    
                }
            }
        }
        return result;
    }
}
