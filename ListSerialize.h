#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <exception>


#define IMAGE_TYPE std::map<std::vector<std::uint32_t>, std::string> 

typedef enum {
    CURRENT,
    PREVIOUS,
    NEXT,
    RANDOM_TO,
    LENGTH,
    SIZE
}META;

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
    std::vector<std::uint32_t> meta{ 0,0,0,0,0 };
    IMAGE_TYPE* Pack(ListNode* node);
    void Associate(IMAGE_TYPE* image, std::map<std::uint32_t, ListNode*>* indexed_list);
    void Serialize(std::ofstream& file_stream);
    void Deserialize(std::ifstream& file_stream);
};