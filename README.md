# FuzzySpellChecker

Fuzzy Spell Checker

- A high-performance C++ spell correction engine utilizing a Trie (Prefix Tree) and Dynamic Programming to perform fuzzy string matching in real-time.

Architecture

- The engine operates by combining two computer science concepts to achieve sub-millisecond search latency:
- Trie (Prefix Tree): Efficiently stores the dictionary, allowing us to traverse character by character.
- Levenshtein Distance (Dynamic Programming): Calculates the "edit distance" between the user's input and dictionary words.
- By performing this calculation during the Trie traversal (and pruning branches that exceed the allowed maxEdits), we avoid checking the entire dictionary, making the search extremely fast.
