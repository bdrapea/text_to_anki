#include "t2a_sqlite.h"

namespace t2a
{
std::string sql_insert_vocabulary(const char* word, const char* meaning)
{
    std::string retval = "INSERT INTO Vocabulary VALUES (";
    retval += '\'';
    retval += word;
    retval += '\'';
    retval += ',';
    retval += '\'';
    retval += meaning;
    retval += '\'';
    retval += ");";

    return retval;
}

void check_database(const boost::filesystem::path& database_path)
{
    sqlite3* tmp_db = nullptr;
    int sql_result = sqlite3_open(database_path.string().c_str(), &tmp_db);
    if (sql_result != SQLITE_OK)
    {
        throw sqlite3_errstr(sql_result);
    }
}

void check_output_database(const boost::filesystem::path& output_db_path)
{
    if (boost::filesystem::exists(output_db_path))
    {
        std::cerr << "Output database already present: deleting it ..."
                  << std::endl;
        remove(output_db_path);
    }
}
}
