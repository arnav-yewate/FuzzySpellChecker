#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <chrono>

// 1. The Trie Node Structure
class TrieNode
{
public:
    std::unordered_map<char, TrieNode *> children;
    bool isEndOfWord;
    std::string word; // Storing the full word here makes retrieval easier later

    TrieNode() : isEndOfWord(false) {}
};

// 2. The Autocomplete Engine
class Fuzzyspellchecker
{
private:
    TrieNode *root;
    int maxEdits; // The maximum number of typos allowed

    // The Recursive DFS + Dynamic Programming Function
    void searchRecursive(TrieNode *node, const std::string &target, const std::vector<int> &previousRow, std::vector<std::pair<int, std::string>> &results)
    {
        int columns = target.length() + 1;

        for (auto const &[character, childNode] : node->children)
        {
            std::vector<int> currentRow(columns);
            currentRow[0] = previousRow[0] + 1;
            int minDistance = currentRow[0];

            for (int c = 1; c < columns; ++c)
            {
                int insertCost = currentRow[c - 1] + 1;
                int deleteCost = previousRow[c] + 1;
                int replaceCost = previousRow[c - 1] + (target[c - 1] != character ? 1 : 0);

                currentRow[c] = std::min({insertCost, deleteCost, replaceCost});
                minDistance = std::min(minDistance, currentRow[c]);
            }

            // Updated: Push both the distance score and the word as a pair
            if (currentRow.back() <= maxEdits && childNode->isEndOfWord)
            {
                results.push_back({currentRow.back(), childNode->word});
            }

            if (minDistance <= maxEdits)
            {
                searchRecursive(childNode, target, currentRow, results);
            }
        }
    }

public:
    Fuzzyspellchecker(int maxE) : maxEdits(maxE)
    {
        root = new TrieNode();
    }

    ~Fuzzyspellchecker() { clearTrie(root); }

    void clearTrie(TrieNode *node)
    {
        for (auto &pair : node->children)
            clearTrie(pair.second);
        delete node;
    }

    // Standard Trie Insertion
    void insert(const std::string &word)
    {
        TrieNode *current = root;
        for (char ch : word)
        {
            if (current->children.find(ch) == current->children.end())
            {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
        current->word = word;
    }

    // Public search function to initialize the DFS
    std::vector<std::string> search(const std::string &target)
    {
        std::vector<int> firstRow(target.length() + 1);
        std::iota(firstRow.begin(), firstRow.end(), 0);

        // Use pairs to capture the results
        std::vector<std::pair<int, std::string>> pairedResults;
        searchRecursive(root, target, firstRow, pairedResults);

        // Sort the results so the lowest edit distance comes first
        std::sort(pairedResults.begin(), pairedResults.end());

        // Extract just the words to send back to the user
        std::vector<std::string> finalResults;
        for (const auto &pair : pairedResults)
        {
            finalResults.push_back(pair.second);
        }

        return finalResults;
    }
};

// 3. The Main Execution File
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

    // ── BENCHMARK 1: Dictionary Loading ──────────────────────
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

    // ── BENCHMARK 2: Search Query ────────────────────────────
    std::string searchTarget;
    std::cout << "Enter a word to search (try making a typo): ";
    std::cin >> searchTarget;

    auto searchStart = std::chrono::high_resolution_clock::now();

    std::vector<std::string> matches = engine.search(searchTarget);

    auto searchEnd = std::chrono::high_resolution_clock::now();
    auto searchDuration = std::chrono::duration_cast<std::chrono::microseconds>(searchEnd - searchStart);

    std::cout << "\nDid you mean:\n";
    if (matches.empty())
    {
        std::cout << "(No matches found within the allowed edit distance)\n";
    }
    else
    {
        for (const std::string &match : matches)
            std::cout << "- " << match << "\n";
    }

    std::cout << "\nSearch time: " << searchDuration.count() << " microseconds ("
              << searchDuration.count() / 1000.0 << " ms)\n";
    std::cout << "Matches found: " << matches.size() << "\n";

    return 0;
}