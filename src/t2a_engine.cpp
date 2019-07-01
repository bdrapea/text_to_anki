#include "t2a_engine.h"

namespace t2a
{
void generate_reference_database_from_goi_pdf(
    const boost::filesystem::path& goi_pdf_path,
    const boost::filesystem::path& reference_database_path)
{
    poppler::document* goi_pdf =
        poppler::document::load_from_file(goi_pdf_path.c_str());

    sqlite3* ref_db = nullptr;
    sqlite3_open(reference_database_path.c_str(), &ref_db);
    sqlite3_close(ref_db);

    std::cout << goi_pdf->create_page(0)->text().to_utf8().data() << std::endl;
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
                sqlite3_exec(out_db,
                             sql_insert_vocabulary(
                                 word,meaning).c_str(),
                             nullptr, nullptr, nullptr);
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

void generate_anki_apkg_from_database(
    const boost::filesystem::path&vocabulary_database_path,
    const boost::filesystem::path& anki_apkg_path)
{
    boost::filesystem::path anki_dir_path = anki_apkg_path.parent_path();
    boost::filesystem::path anki_media_path = anki_dir_path;
    anki_media_path /= "media";
    boost::filesystem::path anki_anki2_path = anki_dir_path;
    anki_anki2_path /= "collection.anki2";

    /** Creating default media file **/
    create_empty_anki_media_file(anki_media_path);

    /** Creating default anki2 file **/
    create_default_anki2_file(anki_anki2_path);

    /** Add db value to anki2 file **/
    add_anki_collection(vocabulary_database_path, anki_anki2_path, "test");

    /** Zipping media file and anki2 file to apkg **/
    create_apkg(anki_anki2_path, anki_media_path, "yy");

    /** Deleting anki2 and media file **/
    remove(anki_media_path);
    remove(anki_anki2_path);
}
}
