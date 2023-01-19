#include "ListSerialize.h"

IMAGE_TYPE* ListRand::Pack(ListNode* node) {
    IMAGE_TYPE* image = new IMAGE_TYPE();
    if (!node) 
        return image;
    std::map<ListNode*, std::uint32_t>* indexed_list = new std::map<ListNode*, std::uint32_t>;
    std::uint32_t index = 1;
    try {
        while (node) {
            indexed_list->insert({ node,index++ });
            if (node->Next && node->Next->Prev != node) break;
            node = node->Next;
        }
        this->Count = --index;
        for (auto iter = indexed_list->begin(); iter != indexed_list->end(); iter++) {
            meta.at(CURRENT) = iter->second;
            meta.at(NEXT) = iter->second + 1;
            meta.at(PREVIOUS) = iter->second - 1;
            meta.at(RANDOM_TO) = indexed_list->at(iter->first->Rand);
            meta.at(LENGTH) = static_cast<std::uint32_t>(iter->first->Data.size());
            image->insert({ meta, iter->first->Data.data() });
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Pack error! Error type: " << typeid(e).name() << std::endl;
    }
    indexed_list->clear();
    delete indexed_list;
    return image;

}


void ListRand::Serialize(std::ofstream& file_stream) {
    if (file_stream.fail()) {
        std::cout << "File error!" << std::endl;
        return;
    }
    IMAGE_TYPE* image = this->Pack(this->Head);
    file_stream.write(reinterpret_cast<const char*>(&(this->Count)), sizeof((this->Count)));
    for (auto iter_image = image->begin(); iter_image != image->end(); iter_image++) {
        for (auto iter_meta = iter_image->first.begin(); iter_meta != iter_image->first.end(); iter_meta++)
            file_stream.write(reinterpret_cast<const char*>(&*iter_meta), sizeof(*iter_meta));
        file_stream.write(iter_image->second.data(), iter_image->second.size());
    }
    image->clear();
    delete image;
}

void ListRand::Deserialize(std::ifstream& file_stream) {
    if (file_stream.fail()) {
        std::cout << "File error!" << std::endl;
        return;
    }
    IMAGE_TYPE* image = new IMAGE_TYPE();
    std::map<std::uint32_t, ListNode*>* indexed_list = new std::map<std::uint32_t, ListNode*>;
    char tmp[sizeof(std::uint32_t)];
    try{
        file_stream.read(tmp, sizeof(std::uint32_t));
        memcpy(&(this->Count), tmp, sizeof(std::uint32_t));
        for (int image_index = 0; image_index < this->Count; image_index++) {
            std::vector<std::uint32_t> vec_tmp{ 0,0,0,0,0 };
            for (int meta_index = 0; meta_index < META::SIZE; meta_index++) {
                file_stream.read(tmp, sizeof(std::uint32_t));
                memcpy(&vec_tmp[meta_index], tmp, sizeof(std::uint32_t));
            }
            char* data_tmp = new char[vec_tmp.at(LENGTH)];
            file_stream.read(data_tmp, vec_tmp.at(LENGTH));
            std::string data(data_tmp, data_tmp + vec_tmp.at(LENGTH));
            image->insert({ vec_tmp,data });
            indexed_list->insert({ vec_tmp.at(CURRENT),new ListNode() });
            delete[] data_tmp;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Deserialize error! Error type: " << typeid(e).name() << std::endl;
    }
    this->Associate(image, indexed_list);
}

void ListRand::Associate(IMAGE_TYPE* image, std::map<std::uint32_t, ListNode*>* indexed_list) {
    ListNode* node = indexed_list->begin()->second;
    this->Head = node;
    std::map<std::uint32_t, ListNode*>::iterator list_iter;
    IMAGE_TYPE::iterator image_iter;
    try {
        for (image_iter = image->begin(), list_iter = indexed_list->begin(); image_iter != image->end() && list_iter != indexed_list->end(); image_iter++, list_iter++) {
            node->Data = image_iter->second;
            node->Rand = indexed_list->at(image_iter->first[RANDOM_TO]);
            if (std::next(list_iter, 1) != indexed_list->end())
                node->Next = (std::next(list_iter, 1))->second;
            else
                this->Tail = node;
            if (list_iter != indexed_list->begin())
                node->Prev = std::prev(list_iter, 1)->second;
            node = node->Next;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Associate error! Error type: " << typeid(e).name() << std::endl;
    }
    image->clear();
    delete image;
    indexed_list->clear();
    delete indexed_list;
}

