#include "t2a_anki.h"

namespace t2a
{
void create_empty_anki_media_file(
    const boost::filesystem::path& anki_media_file_path)
{
    std::ofstream media_file(
        anki_media_file_path.string().c_str(),
        std::ios::out);
    media_file << "{}";
    media_file.close();
}

void create_default_anki2_file(const boost::filesystem::path& anki2_path)
{
    sqlite3* anki2_db = nullptr;
    sqlite3_open(anki2_path.string().c_str(), &anki2_db);

    boost::filesystem::path sql_template_path =
            boost::filesystem::path(__FILE__).parent_path().parent_path();
    sql_template_path /= TEMPLATE_DIR_NAME;
    sql_template_path /= ANKI_SQL_TEMPLATE_FILENAME;
    std::string sql_cmds = load_file_in_string(sql_template_path);
    sqlite3_errstr(sqlite3_exec(anki2_db, sql_cmds.c_str(), nullptr, nullptr, nullptr));

    sqlite3_close(anki2_db);
}

void check_output_anki_apkg(const boost::filesystem::path& anki_apkg_path)
{
    if (boost::filesystem::exists(anki_apkg_path))
    {
        std::cerr << "Output database already present: deleting it ..."
                  << std::endl;
        remove(anki_apkg_path);
    }
}
}
