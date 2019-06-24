#include "t2a_engine.h"

namespace t2a
{
void generate_sub_database_from_string(
    const boost::filesystem::path& reference_database_path,
    const std::string& str,
    const boost::filesystem::path& output_database_path)
{
    sqlite3* ref_db = nullptr;
    sqlite3* out_db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int retval = 0;
    std::string sql_cmd = "";

    sqlite3_open(output_database_path.string().c_str(), &out_db);
    sqlite3_exec(out_db, CREATE_VOCABULARY_TABLE, nullptr, nullptr, nullptr);

    sql_cmd = "SELECT * FROM Vocabulary";
    sqlite3_open(reference_database_path.string().c_str(), &ref_db);
    sqlite3_prepare_v2(ref_db, sql_cmd.c_str(), -1, &stmt, nullptr);
    while (true)
    {
        retval = sqlite3_step(stmt);

        if (retval == SQLITE_ROW)
        {
            const char* word =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, WORD_COLUMN_INDEX));
            const char* meaning =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, MEANING_COLUMN_INDEX));

            if (str.find(word) != std::string::npos)
            {
                sqlite3_errstr( sqlite3_exec(out_db,
                                             sql_insert_vocabulary(word,meaning).c_str(),
                                             nullptr,
                                             nullptr,
                                             nullptr));
            }
        }
        else if (retval == SQLITE_DONE)
        {
            break;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(ref_db);
    sqlite3_close(out_db);
}
}
