#include "DatabaseManager.h"

DatabaseManager * DatabaseManager::instance()
{
    static DatabaseManager instance;
    return &instance;
}

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
}

void DatabaseManager::InitializeTables()
{
    int l_TableBase = 0x006337D0;
    for (int l_I = 0; l_I < 235; l_I++)
    {
        l_TableBase += 0x11;

        int l_Index = *(int*)(l_TableBase + 1);
        int l_TablePtr = *(int*)(l_TableBase + 0xB);
        m_Databases[l_Index] = new Database(l_TablePtr);
    }
}

Database* DatabaseManager::GetDatabaseByIndex(int p_Index)
{
    auto const& l_Itr = m_Databases.find(p_Index);
    if (l_Itr != m_Databases.end())
        return l_Itr->second;

    return nullptr;
}
