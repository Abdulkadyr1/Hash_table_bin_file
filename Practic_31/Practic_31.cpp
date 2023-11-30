#include <iostream>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"
#include <chrono>
#include <fstream>

using namespace std;

int main()
{
    hashtablestruct data;
    setlocale(LC_ALL, "Rus");
    //testBinF();
    //testHeshT();
    /*
    data = from_fail_to_table("Dict.txt", data);
    data.print_all();
    cout << endl;
    delete_from_table_fail("Bye", data);
    data.print_all();
    */
    cout << "Введите как хотите заполнить файл, 5 элементов[1], 500 элементов[2]: ";
    int ch;
    cin >> ch;
    string filename;
    if (ch == 1) {
        filename = "Dict.txt";
    }
    else {
        filename = "Dict500.txt";
    }
    cout << "Выберите функцию From_file_to_table[1], Delete_from_table_and_file[2], Print_table[3], Print_file[4], Time_test[5], Exit[0]:  ";
    int k;
    cin >> k;
    DictionaryEntry entry;
    while (k != 0) {
        if (k == 1) {
            data = from_fail_to_table(filename, data);
        }
        else if (k == 2) {
            cout << "Введите что хотите удалить: ";
            string word;
            cin >> word;
            delete_from_table_fail(word, data, filename);
        }
        else if (k == 3) {
            data.print_all();
        }
        else if (k == 4) {
            show_all(filename, CountLinesInFile(filename));
        }
        
        else if (k == 5) {
            ifstream infile;
            infile.open(filename);
            if (!infile.is_open()) {
                cerr << "Error opening file" << endl;
            }
            auto fileSize = infile.tellg();
            string line;
            const auto start = std::chrono::steady_clock::now();
            if (std::getline(infile, line)) { // Читать первую строку
                const auto end = std::chrono::steady_clock::now(); // Остановить измерение времени
                const std::chrono::duration<double> elapsed_seconds = end - start;
                std::cout << "Elapsed time to read the first line: " << elapsed_seconds.count() << " seconds" << std::endl;
                infile.close();
            }
            else {
                std::cerr << "Error reading the first line or the file is empty" << std::endl;
            }
            //infile.read((char*)&entry, fileSize);
            //const auto end = std::chrono::steady_clock::now();
            //const std::chrono::duration<double> elapsed_seconds = end - start;
            //cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << endl;
        }            
        
        else if (k == 0) {
            break;
        }
        cout << "Выберите функцию From_file_to_table[1], Delete_from_table_and_file[2], Print_table[3], Print_file[4], Time_test[5], Exit[0]: ";
        cin >> k;
    }
    return 0;
}

