#include "node.h"
#include <string>
#include <vector>
#include <queue>
#include <locale>
#include <codecvt>
#include <iostream>

// Constructor for Node.
Node::Node(char b) {
    byte = b;
    for (int i = 0; i < 256; i++) {
        children[i] = nullptr;
    }
}

// Getter for character.
char Node::getByte() {
    return byte;
}

// Getter for child.
Node* Node::getChild(char b) {
    return children[static_cast<unsigned char>(b)];
}

// Add a child to the node.
void Node::addByteSequence(const std::vector<unsigned char>& byteSequence, unsigned int startIndex) {
    // Base case: we are at the end of the word.
    if (startIndex == byteSequence.size()) {
        // Check if the null terminator is already a child.
        if (children[0] == nullptr) {
            children[0] = new Node('\0');
        }
        
        return;
    }

    // Get the next byte.
    int nextByte = byteSequence[startIndex];

    // If the next byte is not in the children, add it.
    if (children[nextByte] == nullptr) {
        children[nextByte] = new Node(nextByte);
    }

    // Add the rest of the word to the child.
    children[nextByte]->addByteSequence(byteSequence, startIndex + 1);
}

// Get all words with a given prefix.
std::vector<std::string> Node::getByteSequence(const std::vector<unsigned char>& prefix, unsigned int startIndex) {
    std::vector<std::string> words = std::vector<std::string>();

    if (startIndex == prefix.size()) {
        // BFS to get the first 10 words.
        std::queue<std::pair<Node*, std::vector<unsigned char>>> q = std::queue<std::pair<Node*, std::vector<unsigned char>>>();

        q.push(std::make_pair(this, prefix));

        while (!q.empty() && words.size() < 10) {
            std::pair<Node*, std::vector<unsigned char>> p = q.front();
            q.pop();

            Node* node = p.first;
            std::vector<unsigned char> sequence = p.second;

            // If we are at the end of a word, add it to the vector.
            if (node->getChild('\0') != nullptr) {
                // Add the word to the vector.
                words.push_back(std::string(sequence.begin(), sequence.end()));
            }

            // Add all children to the queue.
            for (int i = 1; i < 256; i++) {
                Node* child = node->getChild(i);
                if (child != nullptr) {
                    std::vector<unsigned char> newSeq(sequence.begin(), sequence.end());
                    newSeq.push_back(child->getByte());
                    q.push(std::make_pair(child, newSeq));
                }
            }
        }

        return words;
    }

    // Get the next byte.
    int nextByte = prefix[startIndex];

    // If the next byte is not in the children, return an empty vector.
    if (children[nextByte] == nullptr) {
        return words;
    }

    // Get all words from the child.
    return children[nextByte]->getByteSequence(prefix, startIndex + 1);
}
