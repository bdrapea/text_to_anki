#pragma once

namespace t2a
{
constexpr const char* TEMPLATE_DIR_NAME = "templates";
constexpr const char* ANKI_SQL_TEMPLATE_FILENAME =
    "anki2_default_sql_template.sql";
constexpr const char* DEFAULT_CONF_JSON_FILENAME = "default_conf.json";
constexpr const char* DEFAULT_MODELS_JSON_FILENAME = "default_models.json";
constexpr const char* DEFAULT_DECKS_JSON_FILENAME = "default_decks.json";
constexpr const char* DEFAULT_DCONF_JSON_FILENAME = "default_dconf.json";
constexpr const char* DEFAULT_DECKS_ID = "1398130078204";
constexpr const long DEFAULT_MODEL_ID = 1342697561419;
constexpr const int SHA1_CHECKSUM_DIGITS = 10;
constexpr const int WORD_COLUMN_INDEX = 0;
constexpr const int MEANING_COLUMN_INDEX = 1;
constexpr const char* CREATE_VOCABULARY_TABLE =
    "CREATE TABLE Vocabulary (Word TEXT, Meaning TEXT);";
constexpr const char* GOI_PDF_FILENAME = "goi.pdf";
constexpr const int KANA_UNICODE_LOWER_BOUND = 0xe38180;
constexpr const int KANA_UNICODE_UPPER_BOUND = 0xe383bf;
constexpr const int KANJI_UNICODE_LOWER_BOUND = 0xe4b880;
constexpr const int KANJI_UNICODE_UPPER_BOUND = 0xe9bfbf;
}
