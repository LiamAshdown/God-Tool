#pragma once

#include "../Common/SharedDefines.h"
#include "Database.h"


class DatabaseManager
{
public:
    static DatabaseManager* instance();

public:
    DatabaseManager();
    
    ~DatabaseManager();

public:
    void InitializeTables();

    Database* GetDatabaseByIndex(int p_Index);

private:
    std::map<int, Database*> m_Databases;
};

#define sDatabaseMgr DatabaseManager::instance()