#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <exception>

class ListNode {
public:
    ListNode* Prev;
    ListNode* Next;
    ListNode* Rand;
    std::string Data;
};

class ListRand {
public:
    ListNode* Head;
    ListNode* Tail;
    std::uint32_t Count;
    void Serialize(std::ofstream& file_stream);
    void Deserialize(std::ifstream& file_stream);
};