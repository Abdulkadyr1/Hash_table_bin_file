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

// ������� ��� �������������� ��������� ������ �� ���������� ����� � �������� ����
void ConvertTextToBinary(const string& input_file_name, const string& output_file_name) {
    ifstream text_file;
    text_file.open(input_file_name);
    if (!text_file) {
        cout << "������: ��������� ���� �� ������." << endl;
        return;
    }

    ofstream binary_file(output_file_name, ios::binary);
    if (!binary_file) {
        cout << "������: ���������� ������� �������� ����." << endl;
        return;
    }

    string key;
    string english, russian;
    while (text_file >> key >> english >> russian) {
        if (english.size() > 19 || russian.size() > 19) {
            cout << "������: ����� ������� �������." << endl;
            continue;
        }

        DictionaryEntry entry = { key };
        strcpy_s(entry.englishWord, english.c_str());
        strcpy_s(entry.russianWord, russian.c_str());
        binary_file.write(reinterpret_cast<char*>(&entry), sizeof(entry));
    }

    text_file.close();
    binary_file.close();
    cout << "�������������� ���������." << endl;
}

// ������� ��� �������������� ������ �� ��������� ����� � ��������� ����
void ConvertBinaryToText(const string& input_file_name, const string& output_file_name) {
    ifstream binary_file(input_file_name, ios::binary);
    if (!binary_file) {
        cout << "������: �������� ���� �� ������." << endl;
        return;
    }

    ofstream text_file(output_file_name);
    if (!text_file) {
        cout << "������: ���������� ������� ��������� ����." << endl;
        return;
    }

    DictionaryEntry entry;
    while (binary_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        text_file << entry.key << " " << entry.englishWord << " " << entry.russianWord << endl;
    }

    binary_file.close();
    text_file.close();
    cout << "�������������� ���������." << endl;
}

// ������� ��� ������ ���� ������� ��������� �����
void DisplayAllEntries(const string& input_file_name) {
    ifstream binary_file(input_file_name, ios::binary);
    if (!binary_file) {
        cout << "������: �������� ���� �� ������." << endl;
        return;
    }

    DictionaryEntry entry;
    while (binary_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        cout << "���������� �����: " << entry.englishWord << ", ������� �����: " << entry.russianWord << endl;
    }

    binary_file.close();
}

// ������� ��� ������� � ������ �� �� ����������� ������ � �����
void AccessEntryByIndex(const string& input_file_name, int index) {
    ifstream binary_file(input_file_name, ios::binary);
    if (!binary_file) {
        cout << "������: �������� ���� �� ������." << endl;
        return;
    }

    binary_file.seekg(index * sizeof(DictionaryEntry), ios::beg);
    DictionaryEntry entry;
    if (binary_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        cout << "���������� �����: " << entry.englishWord << ", ������� �����: " << entry.russianWord << endl;
    }
    else {
        cout << "������ �� �������." << endl;
    }

    binary_file.close();
}



// ������� ��� �������� ������ � �������� ��������� �����, ������� �� ��������� �������
void DeleteEntryByKey(const string& input_file_name, string key_to_delete) {
    ifstream input_file(input_file_name, ios::binary);
    if (!input_file) {
        cout << "������: �������� ���� �� ������." << endl;
        return;
    }

    ofstream temp_file("temp.bin", ios::binary);
    if (!temp_file) {
        cout << "������: ���������� ������� ��������� ����." << endl;
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
        cout << "������ � ������ " << key_to_delete << " �������." << endl;
    }
    else {
        cout << "������ ��� �������� ������." << endl;
    }
}

// ������� ��� ���������� �������� �������� �� ����������� �����

void UpdateTranslation(const string& filename, const string& englishWord, const string& newRussianTranslation) {
    ifstream file_in(filename);
    if (!file_in) {
        cout << "������: ���� �� ������." << endl;
        return;
    }

    ofstream file_out("temp.txt"); // ��������� ����

    string line;
    bool found = false;

    while (getline(file_in, line)) {
        if (line.find(englishWord) == 2) { // ����� ����������� ����� �� ������� ��������
            line = line.substr(0, 2) + englishWord + " " + newRussianTranslation;
            found = true;
        }
        file_out << line << endl; // ���������� ������ �� ��������� ����
    }

    file_in.close();
    file_out.close();

    if (!found) {
        cout << "����� �� ������� � �����." << endl;
        remove("temp.txt"); // ������� ��������� ����
        return;
    }

    if (remove(filename.c_str()) == 0 && rename("temp.txt", filename.c_str()) == 0) {
        cout << "������� ������� ��������." << endl;
    }
    else {
        cout << "������ ��� ���������� �������� ��������." << endl;
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
    // 1. �������� ��������� F, ������� ������ � ������ filename.
    // ���� ����������� ��� ������ � ��������� �������.
    ifstream F(filename, ios::in);

    // 2. ��������, ���������� �� ����
    if (!F)
    {
        return -1;
    }

    // 3. ��������� ���������� ����� � �����
    // 3.1. �������� �������������� ����������
    int count = 0;
    char buffer[1000]; // ����� ��� ���������� ����� ������

    // 3.2. ���� ������ �����.
    // ������ ������ ����� �������� �������� getline().
    // �������� ����� ����� �������������� �������� eof().
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
        cout << "����: " << entry.key << " ";
        cout << "���������� �����: " << entry.englishWord << " ";
        cout << "������� �����: " << entry.russianWord << endl;
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

    // ������� �������� ����
    std::remove(filename.c_str());

    // ������������� Temp.txt � ��� ��������� �����
    std::rename("Temp.txt", filename.c_str());
}

void testBinF() {
    setlocale(LC_ALL, "Rus");
    system("chcp 1251");
    cout << "������� �������� �������� TextToBinary[1],  BinaryToText[2], ShowAll[3], Find_in_binary[4], Delete_by_key[5], UpdateTranslation[6], Get_by_key[7], Exit[0]: ";
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
            UpdateTranslation("Dict.txt", "Bye", "��");
        }
        else if (k == 7) {
            cout << "������� ���� �� �������� ������ �����: ";
            string key;
            cin >> key;
            Getbykey("Dict.txt", key, CountLinesInFile("Dict.txt"));
        }
        else if (k == 0)
        {
            break;
        }
        cout << "������� �������� �������� TextToBinary[1],  BinaryToText[2], ShowAll[3], Find[4], Delete_by_key[5], UpdateTranslation[6],  Get_by_key[7], Exit[0]: ";
        std::cin >> k;

    }
}