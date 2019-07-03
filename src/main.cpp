#include <boost/program_options.hpp>

#include "t2a_engine.h"
#include "t2a_utils.h"

constexpr const char* REF_DB_PATH =
    "/home/bdrapeaud/Programmation/Cpp/text_to_anki/text_to_anki/test_files/japanese_reference_database.db";

int main(int argc, char** argv)
{
    using namespace std::chrono;
    auto start_time = high_resolution_clock::now();

    if (argv[0] == nullptr || argv[1] == nullptr)
    {
        std::cerr << "Please enter first the text in japanese and then the "
                     "path to the output apkg";

        return -1;
    }

    boost::filesystem::path text_file_path = argv[1];
    boost::filesystem::path anki_apkg_path = argv[2];
    std::string level_selected;

    /** Input integrity checking **/
    try
    {
        t2a::check_file(text_file_path);
        t2a::check_output_anki_apkg(anki_apkg_path);
        if (argv[3] == nullptr)
        {
            throw "Please provide a JLPT level";
        }
        level_selected = argv[3];

        if (level_selected.size() == 2)
        {
            if (t2a::is_jlpt_level(level_selected.data()) == false)
            {
                throw "Please specify correctly the JLPT level you want";
            }
        }
        else
        {
            throw "Please specify correctly the JLPT level you want";
        }
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

    if (boost::filesystem::exists(REF_DB_PATH) == false)
    {
        std::cout << "Creating reference database from goi pdf..." << std::endl;
        t2a::generate_reference_database_from_goi_pdf(
            goi_pdf_path, REF_DB_PATH);
    }

    boost::filesystem::path out_db_path = text_file_path.parent_path();
    out_db_path /= "OUT.db";

    std::string str = t2a::load_file_in_string(text_file_path);
    t2a::generate_sub_database_from_string(REF_DB_PATH, str, out_db_path);
    t2a::generate_anki_apkg_from_database(out_db_path, anki_apkg_path);
    remove(out_db_path);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    std::cout << "Process took: " << duration.count()/1000.0f << "s" << std::endl;

    return 0;
}
