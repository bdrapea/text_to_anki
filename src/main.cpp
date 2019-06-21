#include "t2a_engine.h"

int main()
{
    t2a::generate_sub_database_from_string(
                "/home/bdrapeaud/Programmation/CppProjects/text_to_anki/text_to_anki/test_files/japanese_reference_database.db",
                "/home/bdrapeaud/Programmation/CppProjects/text_to_anki/text_to_anki/test_files/japanese.txt");
    return 0;
}
