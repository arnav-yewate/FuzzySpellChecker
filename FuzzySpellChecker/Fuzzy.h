#ifndef FUZZYSPELLCHECKER_H
#define FUZZYSPELLCHECKER_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <numeric>

class TrieNode
{
public:
    std::unordered_map<char, TrieNode *> children;
    bool isEndOfWord;
    std::string word; // Storing the full word here makes retrieval easier

    TrieNode() : isEndOfWord(false) {}
};

// Spellchecker
class Fuzzyspellchecker
{
private:
    TrieNode *root;
    int maxEdits; // The maximum number of typos allowed

    // The Recursive DFS + DP Function
    void searchRecursive(TrieNode *node, const std::string &target, const std::vector<int> &previousRow, std::vector<std::pair<int, std::string>> &results);

public:
    Fuzzyspellchecker(int maxE);
    ~Fuzzyspellchecker();

    void clearTrie(TrieNode *node);
    // Standard Trie Insertion
    void insert(const std::string &word);

    // Public search function to initialize the DFS
    std::vector<std::string> search(const std::string &target);
};

#endif