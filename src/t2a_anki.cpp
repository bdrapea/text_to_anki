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

    /** Generating sql insert commands **/
    boost::filesystem::path json_templates_dir =
        boost::filesystem::path(__FILE__).parent_path().parent_path();
    json_templates_dir /= TEMPLATE_DIR_NAME;

    boost::filesystem::path conf_json_path = json_templates_dir;
    conf_json_path /= DEFAULT_CONF_JSON_FILENAME;
    boost::filesystem::path models_json_path = json_templates_dir;
    models_json_path /= DEFAULT_MODELS_JSON_FILENAME;
    boost::filesystem::path decks_json_path = json_templates_dir;
    decks_json_path /= DEFAULT_DECKS_JSON_FILENAME;
    boost::filesystem::path dconf_json_path = json_templates_dir;
    dconf_json_path /= DEFAULT_DCONF_JSON_FILENAME;

    boost::property_tree::ptree decks_pt;
    boost::property_tree::read_json(decks_json_path.c_str(), decks_pt);
    long deck_id = micros();
    decks_pt.get_child(DEFAULT_DECKS_ID)
    .put<std::string>("name", collection_name);
    decks_pt.get_child(DEFAULT_DECKS_ID).put<long>("id",deck_id);

    std::stringstream decks_ss;
    boost::property_tree::write_json(decks_ss, decks_pt);

    std::stringstream sql_insert_col;
    sql_insert_col << "INSERT INTO col VALUES(2,1332961200,1398130163295,";
    sql_insert_col << "1398130163168,11,0,0,0,";
    sql_insert_col << '\'' << load_file_in_string(conf_json_path) << "\',";
    sql_insert_col << '\'' << load_file_in_string(models_json_path) << "\',";
    sql_insert_col << '\'' << decks_ss.str() << "\',";
    sql_insert_col << '\'' << load_file_in_string(dconf_json_path) << "\',";
    sql_insert_col << "'{}');";

    const char* word = "Bonjour";
    const char* meaning = "Hello";
    long note_id = millis();
    boost::uuids::uuid guid = boost::uuids::random_generator()();
    std::stringstream sql_insert_notes, sql_insert_cards;
    sql_insert_notes << "INSERT INTO notes VALUES(";
    sql_insert_notes << note_id << ",";
    sql_insert_notes << "'"<< boost::lexical_cast<std::string>(guid) <<"',";
    sql_insert_notes << DEFAULT_MODEL_ID << ",";
    sql_insert_notes << seconds() << ",";
    sql_insert_notes << "-1,'',";
    sql_insert_notes << "'" << word << "'" << "|| CHAR(0x1f) ||";
    sql_insert_notes << " '" << meaning << "',";
    sql_insert_notes << "'" << word <<"',";
    sql_insert_notes << "4077833205" <<",";
    sql_insert_notes << "0,'');";

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);

    long card_id = millis();
    sql_insert_cards << "INSERT INTO cards VALUES(";
    sql_insert_cards << card_id << ",";
    sql_insert_cards << note_id << ",";
    sql_insert_cards << "1398130078204" << ",";
    sql_insert_cards << "0,1398130110,-1,0,0,484332854,0,0,0,0,0,0,0,0,'');";

    std::stringstream sql_all_inserts;
    sql_all_inserts << sql_insert_col.str();
    sql_all_inserts << sql_insert_notes.str();
    sql_all_inserts << sql_insert_cards.str();

    int sql_err = sqlite3_exec(anki_db,
                               sql_all_inserts.str().c_str(),
                               nullptr,
                               nullptr,
                               nullptr);

    std::cout << sqlite3_errstr(sql_err) << std::endl;

    sqlite3_close(anki_db);
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
