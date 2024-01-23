// Trie header file:
#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "node.h"

class Trie {
    private:
        Node* roots[256];

    public:
        Trie();
        void addByteSequence(const std::vector<unsigned char>& byteSequence);
        std::vector<std::string> getByteSequence(const std::vector<unsigned char>& byteSequence);
};

#endif