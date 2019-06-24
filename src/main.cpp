#include "t2a_engine.h"
#include "t2a_utils.h"

constexpr const char* text_file_path =
    "/home/bdrapeaud/Programmation/CppProjects/text_to_anki/text_to_anki/test_files/japanese.txt";

constexpr const char* ref_db_path =
    "/home/bdrapeaud/Programmation/CppProjects/text_to_anki/text_to_anki/test_files/japanese_reference_database.db";

constexpr const char* out_db_path =
    "/home/bdrapeaud/Bureau/japanese_output_database.db";

int main()
{
    /** Input integrity checking **/
    try
    {
        t2a::check_file(text_file_path);
        t2a::check_database(ref_db_path);
        t2a::check_output_database(out_db_path);
    }
    catch (const char* error_msg)
    {
        std::cerr << error_msg << std::endl;
        return 1;
    }

    /** Main algorithm **/
    std::string str = t2a::load_file_in_string(text_file_path);
    t2a::generate_sub_database_from_string(ref_db_path, str, out_db_path);

    return 0;
}
