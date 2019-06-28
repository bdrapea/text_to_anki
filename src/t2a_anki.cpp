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
    sqlite3_open(anki2_path.c_str(), &anki2_db);

    boost::filesystem::path sql_template_path =
        boost::filesystem::path(__FILE__).parent_path().parent_path();
    sql_template_path /= TEMPLATE_DIR_NAME;
    sql_template_path /= ANKI_SQL_TEMPLATE_FILENAME;
    std::string sql_cmds = load_file_in_string(sql_template_path);
    sqlite3_exec(anki2_db, sql_cmds.c_str(), nullptr, nullptr, nullptr);
    sqlite3_close(anki2_db);
}

void add_anki_collection(
    const boost::filesystem::path& vocabulary_db_path,
    const boost::filesystem::path& anki2_path,
    const char* collection_name)
{
    sqlite3* anki_db = nullptr;
    sqlite3_open(anki2_path.c_str(), &anki_db);

    std::string sql_insert_statement =
        sql_insert_anki_collection(1, collection_name);

    sql_insert_statement += sql_insert_vocabulary(vocabulary_db_path);

    int sql_err = sqlite3_exec(anki_db,
                               sql_insert_statement.c_str(),
                               nullptr,
                               nullptr,
                               nullptr);

    std::cout << sqlite3_errstr(sql_err) << std::endl;

    sqlite3_close(anki_db);
}

std::string sql_insert_anki_collection(
    const int id,
    const char* collection_name)
{
    boost::filesystem::path template_path =
        boost::filesystem::path(__FILE__).parent_path().parent_path();
    template_path /= TEMPLATE_DIR_NAME;

    std::string sql_cmd = "INSERT INTO col VALUES(";
    sql_cmd += "4,0,0,0,11,0,0,0,";

    boost::filesystem::path default_conf_json_path = template_path;
    default_conf_json_path /= "default_conf.json";
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_conf_json_path);
    sql_cmd += "\',";

    boost::filesystem::path default_models_json_path = template_path;
    default_models_json_path /= "default_models.json";
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_models_json_path);
    sql_cmd += "\',";

    boost::filesystem::path default_decks_json_path = template_path;
    default_decks_json_path /= "default_decks.json";
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_decks_json_path);
    sql_cmd += "\',";

    boost::filesystem::path default_dconf_json_path = template_path;
    default_dconf_json_path /= "default_dconf.json";
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_dconf_json_path);
    sql_cmd += "\',";
    sql_cmd += "'{}');";

    return sql_cmd;
}

std::string sql_insert_vocabulary(
    const boost::filesystem::path& vocabulary_db_path)
{
    std::string sql_cmd = "";
    sqlite3* voc_db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int retval = 0;

    sqlite3_open(vocabulary_db_path.string().c_str(), &voc_db);
    sqlite3_prepare_v2(voc_db, "SELECT * FROM Vocabulary", -1, &stmt, nullptr);
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

            sql_cmd += "INSERT INTO cards VALUES(";
            sql_cmd += std::to_string(millis());
            sql_cmd += ",";
            sql_cmd += "0,";
            sql_cmd += "1,";
            sql_cmd += "0,";
            sql_cmd += "0,";
            sql_cmd += "0,";
            sql_cmd += "0,";
            sql_cmd += "0,";
            sql_cmd += "0,";
            sql_cmd += "0,";
        }
        else if (retval == SQLITE_DONE)
        {
            break;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(voc_db);
    return sql_cmd;
}

void create_apkg(
    const boost::filesystem::path& anki2_path,
    const boost::filesystem::path& media_path,
    const char* apkg_name)
{
    boost::filesystem::path anki2_dir = anki2_path.parent_path();
    boost::filesystem::path apkg_path = anki2_dir;
    apkg_path /= std::string(apkg_name) + ".apkg";
    std::string zip_command = "zip -j ";
    zip_command += apkg_path.c_str();
    zip_command += ' ';
    zip_command += anki2_path.c_str();
    zip_command += ' ';
    zip_command += media_path.c_str();

    run_process(zip_command.c_str());
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
