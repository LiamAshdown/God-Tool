#include "Database.h"
#include "DBStructures.h"

Database::Database(int p_TablePtr) : m_TablePtr(p_TablePtr), m_ClientDB((WoWClientDBRec*)p_TablePtr)
{
}


Database::~Database()
{
}

int Database::GetRowPtr(int p_Index)
{
    return ((int(__thiscall*)(int, int))0x0065C290)(m_TablePtr, p_Index + 0x18);
}

int Database::GetRowCount() const
{
    return m_ClientDB->numRows;
}

int Database::GetMinIndex() const
{
    return m_ClientDB->minIndex;
}

WoWClientDBRec * Database::GetClientDB() const
{
    return m_ClientDB;
}
