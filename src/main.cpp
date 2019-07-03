#include "t2a_engine.h"
#include "t2a_utils.h"

constexpr const char* text_file_path =
    "/home/bdrapeaud/Programmation/Cpp/text_to_anki/text_to_anki/test_files/japanese.txt";

constexpr const char* ref_db_path =
    "/home/bdrapeaud/Programmation/Cpp/text_to_anki/text_to_anki/test_files/japanese_reference_database.db";

constexpr const char* out_db_path =
    "/home/bdrapeaud/Bureau/japanese_output_database.db";

constexpr const char* anki_apkg_path =
    "/home/bdrapeaud/Bureau/cake.apkg";

int main()
{
    using namespace std::chrono;
    auto start_time = high_resolution_clock::now();

    /** Input integrity checking **/
    try
    {
        t2a::check_file(text_file_path);
        t2a::check_output_database(out_db_path);
        t2a::check_output_anki_apkg(anki_apkg_path);
    }
    catch (const char* error_msg)
    {
        std::cerr << error_msg << std::endl;
        return 1;
    }

    /** Main algorithm **/
    boost::filesystem::path goi_pdf_path =
        boost::filesystem::path(__FILE__).parent_path().parent_path();
    goi_pdf_path /= t2a::TEMPLATE_DIR_NAME;
    goi_pdf_path /= t2a::GOI_PDF_FILENAME;

    if (boost::filesystem::exists(ref_db_path) == false)
    {
        std::cout << "Creating reference database from goi pdf..." << std::endl;
        t2a::generate_reference_database_from_goi_pdf(
            goi_pdf_path, ref_db_path);
    }
    std::string str = t2a::load_file_in_string(text_file_path);
    t2a::generate_sub_database_from_string(ref_db_path, str, out_db_path);
    t2a::generate_anki_apkg_from_database(out_db_path, anki_apkg_path);
    remove(out_db_path);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    std::cout << "Process took: " << duration.count()/1000.0f << "s" << std::endl;

    return 0;
}
