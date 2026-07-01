#include "Fuzzy.h"

// The Recursive DFS + Dynamic Programming Function
void Fuzzyspellchecker:: searchRecursive(TrieNode *node, const std::string &target, const std::vector<int> &previousRow, std::vector<std::pair<int, std::string>> &results)
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

Fuzzyspellchecker:: Fuzzyspellchecker(int maxE) : maxEdits(maxE)
{
    root = new TrieNode();
}

Fuzzyspellchecker:: ~Fuzzyspellchecker() { clearTrie(root); }

void Fuzzyspellchecker:: clearTrie(TrieNode *node)
{
    for (auto &pair : node->children)
        clearTrie(pair.second);
    delete node;
}

void Fuzzyspellchecker:: insert(const std::string &word)
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

std::vector<std::string> Fuzzyspellchecker:: search(const std::string &target)
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
