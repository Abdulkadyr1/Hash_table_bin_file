#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct DictionaryEntry {
    string key;
    char englishWord[50];
    char russianWord[50];
};

// Функция для преобразования текстовых данных из текстового файла в двоичный файл
void ConvertTextToBinary(const string& input_file_name, const string& output_file_name) {
    ifstream text_file;
    text_file.open(input_file_name);
    if (!text_file) {
        cout << "Ошибка: текстовый файл не найден." << endl;
        return;
    }

    ofstream binary_file(output_file_name, ios::binary);
    if (!binary_file) {
        cout << "Ошибка: невозможно создать двоичный файл." << endl;
        return;
    }

    string key;
    string english, russian;
    while (text_file >> key >> english >> russian) {
        if (english.size() > 19 || russian.size() > 19) {
            cout << "Ошибка: слово слишком длинное." << endl;
            continue;
        }

        DictionaryEntry entry = { key };
        strcpy_s(entry.englishWord, english.c_str());
        strcpy_s(entry.russianWord, russian.c_str());
        binary_file.write(reinterpret_cast<char*>(&entry), sizeof(entry));
    }

    text_file.close();
    binary_file.close();
    cout << "Преобразование завершено." << endl;
}

// Функция для преобразования данных из двоичного файла в текстовый файл
void ConvertBinaryToText(const string& input_file_name, const string& output_file_name) {
    ifstream binary_file(input_file_name, ios::binary);
    if (!binary_file) {
        cout << "Ошибка: двоичный файл не найден." << endl;
        return;
    }

    ofstream text_file(output_file_name);
    if (!text_file) {
        cout << "Ошибка: невозможно создать текстовый файл." << endl;
        return;
    }

    DictionaryEntry entry;
    while (binary_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        text_file << entry.key << " " << entry.englishWord << " " << entry.russianWord << endl;
    }

    binary_file.close();
    text_file.close();
    cout << "Преобразование завершено." << endl;
}

// Функция для вывода всех записей двоичного файла
void DisplayAllEntries(const string& input_file_name) {
    ifstream binary_file(input_file_name, ios::binary);
    if (!binary_file) {
        cout << "Ошибка: двоичный файл не найден." << endl;
        return;
    }

    DictionaryEntry entry;
    while (binary_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        cout << "Английское слово: " << entry.englishWord << ", Русское слово: " << entry.russianWord << endl;
    }

    binary_file.close();
}

// Функция для доступа к записи по ее порядковому номеру в файле
void AccessEntryByIndex(const string& input_file_name, int index) {
    ifstream binary_file(input_file_name, ios::binary);
    if (!binary_file) {
        cout << "Ошибка: двоичный файл не найден." << endl;
        return;
    }

    binary_file.seekg(index * sizeof(DictionaryEntry), ios::beg);
    DictionaryEntry entry;
    if (binary_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        cout << "Английское слово: " << entry.englishWord << ", Русское слово: " << entry.russianWord << endl;
    }
    else {
        cout << "Запись не найдена." << endl;
    }

    binary_file.close();
}



// Функция для удаления записи с заданным значением ключа, заменяя ее последней записью
void DeleteEntryByKey(const string& input_file_name, string key_to_delete) {
    ifstream input_file(input_file_name, ios::binary);
    if (!input_file) {
        cout << "Ошибка: двоичный файл не найден." << endl;
        return;
    }

    ofstream temp_file("temp.bin", ios::binary);
    if (!temp_file) {
        cout << "Ошибка: невозможно создать временный файл." << endl;
        return;
    }

    DictionaryEntry entry;

    while (input_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        if (entry.key != key_to_delete) {
            temp_file.write(reinterpret_cast<char*>(&entry), sizeof(entry));
        }
    }

    input_file.close();
    temp_file.close();

    if (remove(input_file_name.c_str()) == 0 && rename("temp.bin", input_file_name.c_str()) == 0) {
        cout << "Запись с ключом " << key_to_delete << " удалена." << endl;
    }
    else {
        cout << "Ошибка при удалении записи." << endl;
    }
}

// Функция для обновления русского перевода по английскому слову

void UpdateTranslation(const string& filename, const string& englishWord, const string& newRussianTranslation) {
    ifstream file_in(filename);
    if (!file_in) {
        cout << "Ошибка: файл не найден." << endl;
        return;
    }

    ofstream file_out("temp.txt"); // Временный файл

    string line;
    bool found = false;

    while (getline(file_in, line)) {
        if (line.find(englishWord) == 2) { // Поиск английского слова по позиции символов
            line = line.substr(0, 2) + englishWord + " " + newRussianTranslation;
            found = true;
        }
        file_out << line << endl; // Записываем строку во временный файл
    }

    file_in.close();
    file_out.close();

    if (!found) {
        cout << "Слово не найдено в файле." << endl;
        remove("temp.txt"); // Удаляем временный файл
        return;
    }

    if (remove(filename.c_str()) == 0 && rename("temp.txt", filename.c_str()) == 0) {
        cout << "Русский перевод обновлен." << endl;
    }
    else {
        cout << "Ошибка при обновлении русского перевода." << endl;
    }
}

void Getbykey(string filename, string key, int size) {
    ifstream infile;
    infile.open(filename);
    DictionaryEntry entry;
    for (int i = 0; i < size; i++) {
        infile >> entry.key >> entry.englishWord >> entry.russianWord;
        if (entry.englishWord == key) {
            cout << entry.russianWord;
        }
    }
    infile.close();
}
int CountLinesInFile(string filename)
{
    // 1. Объявить экземпляр F, который связан с файлом filename.
    // Файл открывается для чтения в текстовом формате.
    ifstream F(filename, ios::in);

    // 2. Проверка, существует ли файл
    if (!F)
    {
        return -1;
    }

    // 3. Вычислить количество строк в файле
    // 3.1. Объявить дополнительные переменные
    int count = 0;
    char buffer[1000]; // буфер для сохранения одной строки

    // 3.2. Цикл чтения строк.
    // Каждая строка файла читается функцией getline().
    // Проверка конца файла осуществляется функцией eof().
    while (!F.eof())
    {
        count++;

        F.getline(buffer, 1000);
    }
    F.close();
    return count;
}

void show_all(string filename, int size) {
    ifstream infile;
    infile.open(filename);
    DictionaryEntry entry;
    for (int i = 0; i < size; i++) {
        infile >> entry.key >> entry.englishWord >> entry.russianWord;
        cout << "Ключ: " << entry.key << " ";
        cout << "Английское слово: " << entry.englishWord << " ";
        cout << "Русское слово: " << entry.russianWord << endl;
    }
    infile.close();
}

void delete_from_file(string filename, string key, int size) {
    ifstream infile;
    infile.open(filename);
    ofstream outfile1;
    DictionaryEntry entry;
    outfile1.open("Temp.txt");
    for (int i = 0; i < size; i++) {
        infile >> entry.key >> entry.englishWord >> entry.russianWord;
        if (entry.englishWord == key) {
            continue;
        }
        else {
            outfile1 << entry.key << " " << entry.englishWord <<" " << entry.russianWord << endl;
        }
    }
    infile.close();
    outfile1.close();

    // Удалить исходный файл
    std::remove(filename.c_str());

    // Переименовать Temp.txt в имя исходного файла
    std::rename("Temp.txt", filename.c_str());
}

void testBinF() {
    setlocale(LC_ALL, "Rus");
    system("chcp 1251");
    cout << "Введите название операции TextToBinary[1],  BinaryToText[2], ShowAll[3], Find_in_binary[4], Delete_by_key[5], UpdateTranslation[6], Get_by_key[7], Exit[0]: ";
    int k;
    std::cin >> k;
    while (k != 0)
    {
        if (k == 1)
        {
            ConvertTextToBinary("Dict.txt", "dictionary.bin");
        }
        else if (k == 2)
        {
            ConvertBinaryToText("dictionary.bin", "dictionary_text.txt");
        }
        else if (k == 3)
        {
            DisplayAllEntries("dictionary.bin");
        }
        else if (k == 4)
        {
            AccessEntryByIndex("dictionary.bin", 1);
        }
        else if (k == 5)
        {
            DeleteEntryByKey("dictionary.bin", "2");
        }
        else if (k == 6)
        {
            UpdateTranslation("Dict.txt", "Bye", "лл");
        }
        else if (k == 7) {
            cout << "Введите ключ по которому хотите найти: ";
            string key;
            cin >> key;
            Getbykey("Dict.txt", key, CountLinesInFile("Dict.txt"));
        }
        else if (k == 0)
        {
            break;
        }
        cout << "Введите название операции TextToBinary[1],  BinaryToText[2], ShowAll[3], Find[4], Delete_by_key[5], UpdateTranslation[6],  Get_by_key[7], Exit[0]: ";
        std::cin >> k;

    }
}