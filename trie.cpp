#include "trie.h"
#include "node.h"

// Constructor
Trie::Trie() {
    for (int i = 0; i < 256; i++) {
        roots[i] = nullptr;
    }
}

// Add a word to the trie.
void Trie::addByteSequence(const std::vector<unsigned char>& byteSequence) {
    int firstByte = byteSequence[0];

    if (roots[firstByte] == nullptr) {
        roots[firstByte] = new Node(firstByte);
    }


    roots[firstByte]->addByteSequence(byteSequence, 1);
}

// Get all words with a given prefix and within a given distance.
std::vector<std::string> Trie::getByteSequence(const std::vector<unsigned char>& prefix, unsigned int num_autocomplete) {
    int firstByte = prefix[0];

    if (roots[firstByte] == nullptr) {
        return std::vector<std::string>();
    }

    return roots[firstByte]->getByteSequence(prefix, 1, num_autocomplete);
}