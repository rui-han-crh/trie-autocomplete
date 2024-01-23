#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class Node{
    private:
        char byte;
        Node* children[256];

    public:
        Node(char b);
        char getByte();
        Node* getChild(char b);
        void addByteSequence(const std::vector<unsigned char>& byteSequence, unsigned int startIndex);
        std::vector<std::string> getByteSequence(const std::vector<unsigned char>& prefix, unsigned int startIndex);
};

#endif
