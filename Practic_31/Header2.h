#pragma once
#include "Header.h"
#include "Header2.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>



hashtablestruct from_fail_to_table(string input_file_name, hashtablestruct data) {
    ifstream text_file;
    text_file.open(input_file_name);
    DictionaryEntry entry;
    string key;
    string english, russian;
    while (text_file >> key >> english >> russian) {
        data.add(english, russian);
    }
    text_file.close();
    return data;
}

void delete_from_table_fail(string key, hashtablestruct data, string filename) {
    data._delete(key);
    delete_from_file(filename, key, CountLinesInFile(filename));
}


