#include "Database.h"

Database::Database(int p_TablePtr) : m_TablePtr(p_TablePtr)
{
}


Database::~Database()
{
}

int Database::GetRowPtr(int p_Index)
{
    return ((int(__thiscall*)(int, int))0x0065C290)(m_TablePtr, p_Index + 0x18);
}
