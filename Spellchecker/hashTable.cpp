//
// Created by lu on 9/14/2019.
//

#include "hashTable.h"
#include <iostream>

using namespace std;

hashTable::hashTable(int size): capacity(getPrime(size)), filled(0), data(capacity) {}

int hashTable::insert(const string &key, void *pv)
{
    int pos = findPos(key);
    if(pos == -1)
    {
        pos = hash(key);
        while(data[pos].isOccupied)
        {
            if(data[pos].key == key)
                break;
            ++pos %= capacity;
        }
        data[pos].key = key;
        data[pos].isOccupied = true;
        data[pos].isDeleted = false;
        data[pos].pv = pv;

        if(++filled > (capacity / 2))
        {
            if(!rehash())
                return 2;
        }
        return 0;

    } else {
        data[pos].pv = pv;
        return 1;
    }
}

bool hashTable::contains(const string &key)
{
    return (findPos(key) != -1);
}

void * hashTable::getPointer(const string &key, bool *b)
{
    int pos = findPos(key);
    if(pos == -1)
    {
        *b = false;
        return nullptr;
    }
    *b = true;
    return data[pos].pv;
}

int hashTable::setPointer(const string &key, void *pv)
{
    int pos = findPos(key);
    if(pos == -1)
        return 1;
    data[pos].pv = pv;
    return 0;
}

bool hashTable::remove(const string &key)
{
    int pos = findPos(key);
    if(pos == -1)
        return false;
    data[pos].isDeleted = true;
    return true;
}

// Source: Stack Overflow
unsigned int hashTable::hash(const string &key)
{
    unsigned int a = 54059;
    unsigned int b = 76963;
    unsigned int c = 86969;
    unsigned int hash = 37;
    const char* s = key.c_str();
    while(*s)
    {
        hash = (hash * a) ^ (s[0] * b);
        s++;
    }

    return hash % capacity;
}

int hashTable::findPos(const string &key)
{
    int pos = hash(key);
    while(data[pos].isOccupied)
    {
        if(data[pos].key == key)
        {
            if(!data[pos].isDeleted)
                return pos;
            else
                return -1;
        }
        ++pos %= capacity;
    }
    return -1;
}

bool hashTable::rehash()
{
    vector<hashItem> tmp = data;
    data.clear();
    capacity = getPrime(capacity);
    data.resize(capacity);
    if(data.size() != capacity)
        return false;
    filled = 0;
    for(auto& item : tmp)
    {
        if(item.isOccupied && !item.isDeleted)
            insert(item.key, item.pv);
    }
    return true;
}

unsigned int hashTable::getPrime(int size)
{
    static vector<unsigned int> primes = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613,
                                          393241, 786433, 1572869, 3145739, 786433, 1572869, 3145739, 6291469, 12582917,
                                          25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

    for(unsigned int prime : primes)
        if(prime > size * 2)
            return prime;

    return primes[primes.size() - 1];
}
