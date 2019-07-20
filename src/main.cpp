#include "t2a_engine.h"

int main()
{
    const int argc = 5;
    char* argv[argc] =
    {
        "./text_to_anki",
        "-i",
        "/home/bdrapeaud/Programmation/Cpp/text_to_anki/text_to_anki/test_files/japanese.txt",
        "-o",
        "/home/bdrapeaud/Bureau/test.apkg"
    };

    try
    {
        boost::property_tree::ptree normalized_inputs =
                t2a::check_and_normalize_console_inputs(argc, argv);
    }
    catch (const std::string& err_str)
    {
        std::cerr << err_str << std::endl;
    }
    return 0;
}
