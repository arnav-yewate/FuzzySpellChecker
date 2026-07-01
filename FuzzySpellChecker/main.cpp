#include "Fuzzy.h"
//#include "Fuzzy.cpp"
#include <chrono>


int main()
{
    // Initialize engine with a maximum of 2 typos allowed
    Fuzzyspellchecker engine(2);

    std::ifstream file("dictionary.txt");
    std::string word;

    if (!file.is_open())
    {
        std::cout << "Error: Could not open dictionary.txt\n";
        return 1;
    }

    // BENCHMARK 1: Dictionary Loading 
    std::cout << "Loading dictionary into the Trie...\n";

    auto loadStart = std::chrono::high_resolution_clock::now();

    int wordCount = 0;
    while (file >> word)
    {
        engine.insert(word);
        wordCount++;
    }

    auto loadEnd = std::chrono::high_resolution_clock::now();
    auto loadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(loadEnd - loadStart);

    std::cout << "Dictionary loaded successfully!\n";
    std::cout << "Words loaded: " << wordCount << "\n";
    std::cout << "Load time: " << loadDuration.count() << " ms\n\n";

    // BENCHMARK 2: Search Query 
    std::string searchTarget;
    std::cout << "Enter a word to search (try making a typo): ";
    std::cin >> searchTarget;

    auto searchStart = std::chrono::high_resolution_clock::now();

    std::vector<std::string> matches = engine.search(searchTarget);

    auto searchEnd = std::chrono::high_resolution_clock::now();
    auto searchDuration = std::chrono::duration_cast<std::chrono::microseconds>(searchEnd - searchStart);

    
    if (matches.empty())
    {
        std::cout << "(No matches found within the allowed edit distance)\n";
    }
    else
    {
        std::cout << "\nDid you mean:\n";
        for (const std::string &match : matches)
            std::cout << "- " << match << "\n";
    }

    std::cout << "\nSearch time: " << searchDuration.count() << " microseconds ("
              << searchDuration.count() / 1000.0 << " ms)\n";
    std::cout << "Matches found: " << matches.size() << "\n";

    return 0;
}