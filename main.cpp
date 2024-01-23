#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <vector>
#include "trie.h"

const std::string CORPUS = "corpus";

Trie* trie;

void readFromConsole(unsigned int num_autocomplete);
int readFile(const std::filesystem::path filePath);

int main(int argc, char** argv) {
    unsigned int num_autocomplete = 10;

    if (argc > 1) {
        try {
            num_autocomplete = std::stoi(argv[1]);
        } catch (std::invalid_argument& e) {
            std::cout << "Invalid argument: " << argv[1] << std::endl;
            return 1;
        }
    }
    
    trie = new Trie();

    std::cout << "Building trie..." << std::endl;
    std::cout << "\tPlease do not touch the keyboard." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // For all the files in the corpus folder, read each file.
    for (const auto& entry : std::filesystem::directory_iterator(CORPUS)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            readFile(entry.path());
        }
    }

    std::cout << "Trie built." << std::endl;

    std::chrono::duration<double> timeTaken = std::chrono::high_resolution_clock::now() - start;

    std::cout << "\tTime taken: " << std::fixed << std::setprecision(2) << timeTaken.count() << " seconds" << std::endl;

    readFromConsole(num_autocomplete);

    return 0;
}

int readFile(const std::filesystem::path filePath) {
    // Open the file
    std::ifstream file(filePath);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cout << "Error opening file " << filePath << std::endl;
        return 1;
    }

    // Set up a codecvt facet for converting multibyte characters to byte sequences
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<char32_t>);
    file.imbue(utf8_locale);

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        // Vector to store the unsigned char representation of each character
        std::vector<unsigned char> bytes;

        // Convert each character to a byte sequence
        for (unsigned char c : line) {
            // If the character is a newline, skip it
            if (c == '\n') {
                continue;
            }

            bytes.push_back(c);
        }

        // Build the trie using the byte sequence
        trie->addByteSequence(bytes);
    }

    // Close the file
    file.close();

    return 0;
}

// Read from the console.
void readFromConsole(unsigned int num_autocomplete) {
    // Set up a locale for converting multibyte characters to byte sequences.
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<char32_t>);
    std::cin.imbue(utf8_locale);

    std::cout << "> " << std::flush;

    std::string line;
    while (std::getline(std::cin, line)) {
        // Clear the screen.
        std::cout << "\033[2J\033[1;1H";

        // Convert the line to lowercase.
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);

        // Convert each character to a byte sequence.
        std::vector<unsigned char> bytes;
        for (unsigned char c : line) {
            bytes.push_back(c);
        }

        // Get the words.
        std::vector<std::string> words = trie->getByteSequence(bytes, num_autocomplete);

        // Print the prefix.
        std::cout << "> " << line << std::endl;

        // Print the words.
        for (std::string word : words) {
            std::cout << word << std::endl;
        }

        std::cout << std::endl;

        std::cout << "> " << std::flush;
    }

    delete trie;

    return;
}
