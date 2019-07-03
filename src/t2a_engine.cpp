#include "t2a_engine.h"

namespace t2a
{
void generate_reference_database_from_goi_pdf(
    const boost::filesystem::path& goi_pdf_path,
    const boost::filesystem::path& reference_database_path)
{
    poppler::document* goi_pdf =
        poppler::document::load_from_file(goi_pdf_path.c_str());

    std::string sql_cmd = "";
    sql_cmd += CREATE_VOCABULARY_TABLE;

    sqlite3* ref_db = nullptr;
    sqlite3_open(reference_database_path.c_str(), &ref_db);
    sqlite3_exec(ref_db, sql_cmd.c_str(), nullptr, nullptr, nullptr);
    sql_cmd = "";
    int page_count = goi_pdf->pages();

    sql_cmd += "INSERT INTO Vocabulary (Word, Meaning) VALUES";

    for (int i = 0; i < page_count; i++)
    {
        std::stringstream buff(
            goi_pdf->create_page(i)->text().to_utf8().data());
        std::string line;

        // Skip header
        do
        {
            std::getline(buff, line);
        }
        while (!is_jlpt_level(line.data()));

        std::string kana = "";

        while (line.find("Page") == std::string::npos)
        {
            // Read the data
            std::stringstream line_buf(line);
            std::string tmp = "";
            std::string kanji = "";
            std::string meaning = "";
            while (line_buf >> tmp)
            {
                // Extract word
                const char* character = tmp.data();

                if (is_jlpt_level(character))
                {
                    kana = "";
                }
                else if (is_kanji(character))
                {
                    kanji += character;
                }
                else if (is_kana(character))
                {
                    kana += character;
                }
                else
                {
                    if (meaning.empty())
                    {
                        meaning += character;
                    }
                    else
                    {
                        meaning += " ";
                        meaning += character;
                    }
                }
            }

            std::getline(buff, line);

            if (is_jlpt_level(line.data()) == false)
            {
                if (line.find("Page") != std::string::npos)
                {
                    break;
                }
                else
                {
                    std::stringstream(line) >> tmp;
                    if (is_kana(tmp.data()))
                    {
                        kana += tmp;
                    }
                    else
                    {
                        meaning += " ";
                        meaning += tmp;

                    }
                }
            }

            sql_cmd += "(";
            sql_cmd += "'";
            sql_cmd += kanji;

            if (kanji.empty() == false)
            {
                sql_cmd += " ";
            }

            sql_cmd += kana;
            sql_cmd += "'";
            sql_cmd += ",";
            sql_cmd += "'";
            if (meaning.find("'") != std::string::npos)
            {
                boost::algorithm::replace_all(meaning, "'", "''");
            }
            sql_cmd += meaning;
            sql_cmd += "'";
            sql_cmd += "),\n";
        }
    }

    sql_cmd.pop_back();
    sql_cmd.pop_back();
    sql_cmd += ";";
    sqlite3_exec(ref_db, sql_cmd.c_str(), nullptr, nullptr, nullptr);
    std::cout << sqlite3_errmsg(ref_db) << std::endl;
    sqlite3_close(ref_db);
}

void generate_sub_database_from_string(
    const boost::filesystem::path& reference_database_path,
    const std::string& str,
    const boost::filesystem::path&vocabulary_database_path)
{
    sqlite3* ref_db = nullptr;
    sqlite3* out_db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int retval = 0;
    std::string sql_cmd = "";

    sqlite3_open(vocabulary_database_path.string().c_str(), &out_db);
    sqlite3_exec(out_db, CREATE_VOCABULARY_TABLE, nullptr, nullptr, nullptr);

    sql_cmd = "SELECT * FROM Vocabulary";
    sqlite3_open(reference_database_path.string().c_str(), &ref_db);
    sqlite3_prepare_v2(ref_db, sql_cmd.c_str(), -1, &stmt, nullptr);

    sql_cmd = "INSERT INTO Vocabulary (Word, Meaning) VALUES";
    while (true)
    {
        retval = sqlite3_step(stmt);

        if (retval == SQLITE_ROW)
        {
            std::string word =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, WORD_COLUMN_INDEX));
            std::string meaning =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, MEANING_COLUMN_INDEX));

            size_t space_index = word.find(" ");

            if (str.find(word.substr(0, space_index)) != std::string::npos)
            {

                sql_cmd += "(";
                sql_cmd += "'";
                sql_cmd += word;
                sql_cmd += "'";
                sql_cmd += ",";
                sql_cmd += "'";
                if (meaning.find("'") != std::string::npos)
                {
                    boost::algorithm::replace_all(meaning, "'", "''");
                }
                sql_cmd += meaning;
                sql_cmd += "'";
                sql_cmd += "),";
            }
        }
        else if (retval == SQLITE_DONE)
        {
            break;
        }
    }

    sql_cmd.pop_back();
    sql_cmd += ";";

    std::cout << sql_cmd << std::endl;
    sqlite3_exec(out_db, sql_cmd.c_str(), nullptr, nullptr, nullptr);
    sqlite3_finalize(stmt);
    sqlite3_close(ref_db);
    sqlite3_close(out_db);
}

void generate_anki_apkg_from_database(
    const boost::filesystem::path&vocabulary_database_path,
    const boost::filesystem::path& anki_apkg_path)
{
    boost::filesystem::path anki_dir_path = anki_apkg_path.parent_path();
    boost::filesystem::path anki_media_path = anki_dir_path;
    anki_media_path /= "media";
    boost::filesystem::path anki_anki2_path = anki_dir_path;
    anki_anki2_path /= "collection.anki2";

    const char* apkg_stem = anki_apkg_path.stem().c_str();

    /** Creating default media file **/
    create_empty_anki_media_file(anki_media_path);

    /** Creating default anki2 file **/
    create_default_anki2_file(anki_anki2_path);

    /** Add db value to anki2 file **/
    add_anki_collection(vocabulary_database_path,
                        anki_anki2_path,
                        apkg_stem);

    /** Zipping media file and anki2 file to apkg **/
    create_apkg(anki_anki2_path, anki_media_path, apkg_stem);

    /** Deleting anki2 and media file **/
    remove(anki_media_path);
    remove(anki_anki2_path);
}
}
