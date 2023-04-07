#include "ListSerialize.h"

void ListRand::Serialize(std::ofstream& file_stream) {
    if (file_stream.fail()) {
        std::cout << "File error!" << std::endl;
        return;
    }
    ListNode* node = this->Head;
    std::uint32_t index = 1;
    std::unordered_map<ListNode*, std::uint32_t>* indexed_list = new std::unordered_map<ListNode*, std::uint32_t>;
    try {
        while (node) {
            indexed_list->insert({ node,index++ });
            if (node->Next && node->Next->Prev != node) break; // check for looping of list
            node = node->Next;
        }
        file_stream.write(reinterpret_cast<const char*>(&(--index)), sizeof((this->Count)));
        index = 0;
        for (auto iter_list = indexed_list->begin(); iter_list != indexed_list->end(); iter_list++) {
            //previous index
            index = iter_list->second - 1;
            file_stream.write(reinterpret_cast<const char*>(&index), 4);
            //current index
            index = iter_list->second;
            file_stream.write(reinterpret_cast<const char*>(&index), 4);
            //next index
            index = iter_list->second + 1;
            file_stream.write(reinterpret_cast<const char*>(&index), 4);
            //rand index
            index = indexed_list->at(iter_list->first->Rand);
            file_stream.write(reinterpret_cast<const char*>(&index), 4);
            //length of data
            std::uint32_t data_size = iter_list->first->Data.size();
            file_stream.write(reinterpret_cast<const char*>(&data_size), 4);
            //data itself
            file_stream.write(iter_list->first->Data.data(), iter_list->first->Data.size());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Serialize error! Error type: " << typeid(e).name() << std::endl;
    }
    indexed_list->clear();
    delete indexed_list;
}

void ListRand::Deserialize(std::ifstream& file_stream) {
    if (file_stream.fail()) {
        std::cout << "File error!" << std::endl;
        return;
    }
    std::unordered_map<std::uint32_t, ListNode*> indexed_list;
    char tmp[sizeof(std::uint32_t)];
    std::uint32_t prev;
    std::uint32_t current;
    std::uint32_t next;
    std::uint32_t length;
    std::uint32_t rand;

    try {
        file_stream.read(tmp, sizeof(std::uint32_t));
        memcpy(&(this->Count), tmp, sizeof(std::uint32_t));
        for (int i = 1; i <= this->Count; i++) {
            indexed_list[i] = new ListNode();
        }
        this->Head = indexed_list[1];
        this->Tail = indexed_list[this->Count];

        for (int i = 1; i <= this->Count; i++) {
            file_stream.read(tmp, sizeof(std::uint32_t));
            memcpy(&prev, tmp, sizeof(std::uint32_t));

            file_stream.read(tmp, sizeof(std::uint32_t));
            memcpy(&current, tmp, sizeof(std::uint32_t));

            file_stream.read(tmp, sizeof(std::uint32_t));
            memcpy(&next, tmp, sizeof(std::uint32_t));

            file_stream.read(tmp, sizeof(std::uint32_t));
            memcpy(&rand, tmp, sizeof(std::uint32_t));

            file_stream.read(tmp, sizeof(std::uint32_t));
            memcpy(&length, tmp, sizeof(std::uint32_t));

            char* data_tmp = new char[length];
            file_stream.read(data_tmp, length);
            std::string data(data_tmp, data_tmp + length);

            indexed_list[i]->Data = data;
            if (next <= this->Count) indexed_list[i]->Next = indexed_list[next];
            if (!prev) indexed_list[i]->Prev = indexed_list[prev];
            indexed_list[i]->Rand = indexed_list[rand];


        }
    }
    catch (const std::exception& e) {
        std::cerr << "Deserialize error! Error type: " << typeid(e).name() << std::endl;
    }
}

