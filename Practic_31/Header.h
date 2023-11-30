#pragma once
#include <string>
#include <iostream>

using namespace std;

struct mystruct
{
    std::string value = "";
    std::string key = "";
};



struct hashtablestruct
{
    int capacity = 32;
    mystruct* data = new mystruct[capacity + 100];
    int count = 0;

    int myhash(std::string key)
    {
        unsigned long size = key.length();
        int hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash += key[i] * (size + 1);
        }
        hash /= size;
        return hash;
    }
    int rehash(std::string key)
    {
        //        unsigned long size = key.length();
        int hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash += key[i] * 2;
        }

        return hash;
    }
    void realloc(int hash) {
        if (hash >= capacity) {
            int oldCapacity = capacity;
            capacity = hash * 2;
            mystruct* newData = new mystruct[capacity];
            for (int i = 0; i < oldCapacity; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
    }

    void add(std::string key, std::string value) {
        mystruct _struct = mystruct();
        _struct.key = key;
        _struct.value = value;
        add(_struct);
    }

    void add(mystruct node) {
        int hash = myhash(node.key);
        realloc(hash);
        if (this->data[hash].key == "") {
            this->data[hash].key = node.key;
            this->data[hash].value = node.value;

        }
        else {
            this->data[hash + 1].key = node.key;
            this->data[hash + 1].value = node.value;
        }
        this->count += 1;
    }
    std::string get(std::string key) {

        int hash = myhash(key);
        if (this->data[hash].key == key) {
            return this->data[hash].value;
        }
        else {
            return this->data[hash + 1].value;
        }
    }

    void rehash_table() {
        mystruct* newData = new mystruct[capacity];
        for (int i = 0; i < capacity; i++) {
            if (data[i].key != "") {
                int hash = rehash(data[i].key);
                newData[hash] = data[i];
            }
        }
        delete[] data;
        data = newData;
    }

    void print_all() {
        int k = 0;
        for (int i = 0; i < capacity; i++) {
            if (data[i].key == "") {
                continue;
            }
            cout << i << " " << data[i].key << " ";
            cout << data[i].value << endl;
            k++;
            if (k == count) {
                break;
            }
        }
    }
    void _delete(string key) {
        //int hash = myhash(key);
        for (int i = 0; i <= capacity; i++) {
            if (data[i].key == key) {
                data[i].key = "";
                data[i].value = "";
                break;
            }
        }
    }
};

void testHeshT() {
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");
    hashtablestruct table;
    cout << "Введите номер операции Add[1] Get[2] Print_all[3] Exit[0]: ";
    int k;
    cin >> k;
    while (k != 0) {
        if (k == 1) {
            cout << "Введите ключи и значение: ";
            string key;
            string value;
            cin >> key >> value;
            table.add(key, value);
        }
        if (k == 2) {
            cout << "Введите ключ по которому хотите получить значение: ";
            string key;
            cin >> key;
            cout << table.get(key) << endl;
        }
        if (k == 3) {
            table.print_all();
        }
        if (k == 0) {
            break;
        }
        cout << "Введите номер операции Add[1] Get[2] Print_all[3] Exit[0]: ";
        cin >> k;
    }
}

