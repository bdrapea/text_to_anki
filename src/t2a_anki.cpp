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

    std::string deck_id = "";

    std::string sql_insert_statement =
        sql_insert_anki_collection(4, collection_name, &deck_id);

    sql_insert_statement +=
        sql_insert_vocabulary(vocabulary_db_path, deck_id);

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
    const char* collection_name,
    std::string* deck_id)
{
    boost::filesystem::path template_path =
        boost::filesystem::path(__FILE__).parent_path().parent_path();
    template_path /= TEMPLATE_DIR_NAME;

    std::string sql_cmd = "INSERT INTO col VALUES(";
    sql_cmd += std::to_string(id);
    sql_cmd += ",1332961200,1398130163295,1398130163168,11,0,0,0,";

    boost::filesystem::path default_conf_json_path = template_path;
    default_conf_json_path /= DEFAULT_CONF_FILENAME;
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_conf_json_path);
    sql_cmd += "\',";

    boost::filesystem::path default_models_json_path = template_path;
    default_models_json_path /= DEFAULT_MODELS_FILENAME;
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_models_json_path);
    sql_cmd += "\',";

    boost::filesystem::path default_decks_json_path = template_path;
    default_decks_json_path /= DEFAULT_DECKS_FILENAME;
    std::string default_decks_str =
        load_file_in_string(default_decks_json_path);

    boost::property_tree::ptree decks_pt;
    boost::property_tree::read_json(default_decks_json_path.c_str(), decks_pt);

    decks_pt.get_child(DEFAULT_JSON_KEY).
    put<std::string>("name", collection_name);

    long num_deck_id = micros();
    *deck_id = std::to_string(num_deck_id);
    decks_pt.get_child(DEFAULT_JSON_KEY).
    put<long>("id", num_deck_id);

    sql_cmd += '\'';
    sql_cmd += default_decks_str;
    sql_cmd += "\',";

    boost::filesystem::path default_dconf_json_path = template_path;
    default_dconf_json_path /= DEFAULT_DCONF_FILENAME;
    sql_cmd += '\'';
    sql_cmd += load_file_in_string(default_dconf_json_path);
    sql_cmd += "\',";
    sql_cmd += "'{}');";

    return sql_cmd;
}

std::string sql_insert_vocabulary(
    const boost::filesystem::path& vocabulary_db_path,
    const std::string& deck_id)
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
            const boost::uuids::uuid guid = boost::uuids::random_generator()();
            const std::string note_id = std::to_string(micros());
            const std::string note_mod = std::to_string(millis());

            boost::uuids::detail::sha1 sha1;
            unsigned int hash[5] = {0};
            sha1.process_bytes(NOTES_SHA1_TEXT, sizeof (NOTES_SHA1_TEXT-1));
            sha1.get_digest(hash);
            std::string sha1_str = "";
            for (size_t i = 0; i < sizeof(hash) / sizeof(hash[0]); ++i)
            {
                sha1_str += std::to_string(hash[i]);
            }
            sha1_str = sha1_str.substr(0, NOTES_SHA1_DIGITS);

            sql_cmd += "INSERT INTO notes VALUES(";
            sql_cmd += note_id;
            sql_cmd += ",'";
            sql_cmd += boost::lexical_cast<std::string>(guid);
            sql_cmd += "\'";
            sql_cmd += ", 1342697561419,";
            sql_cmd += note_mod;
            sql_cmd += ",-1,'',";
            sql_cmd += "\'";
            sql_cmd += word;
            sql_cmd += '\x1f';
            sql_cmd += meaning;
            sql_cmd += "\',";
            sql_cmd += "\'";
            sql_cmd += meaning;
            sql_cmd += "\',";
            sql_cmd += sha1_str;
            sql_cmd += ",";
            sql_cmd += "0,'');";

            sql_cmd += "INSERT INTO cards VALUES(";
            sql_cmd += std::to_string(micros());
            sql_cmd += ",";
            sql_cmd += note_id;
            sql_cmd += ",";
            sql_cmd += deck_id;
            sql_cmd += ",";
            sql_cmd += "0";
            sql_cmd += ",";
            sql_cmd += note_mod;
            sql_cmd += "-1,0,0,484332854,0,0,0,0,0,0,0,0,'');";

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
