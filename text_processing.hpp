#pragma once

#include <deque>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "util.hpp"

class WordsStatistics {
   public:
    WordsStatistics();

    void addWord(std::string word);

    int64_t getWordsCount();

    const std::deque<std::string>& getUniqueWordsSequence();

    const std::unordered_set<std::string>& getUniqueWords();

   private:
    std::deque<std::string> uniqueWordsSequence;
    std::unordered_set<std::string> wordsInSequence;
    std::unordered_set<std::string> uniqueWords;
    int64_t wordsCount;
};

// word can't contains two consequtive '
class WordsBufferedReader {
   public:
    std::vector<std::string> readWords(const std::string& data,
                                       bool noMoreData = false);

   private:
    std::string buffer;

    std::optional<std::string> splitBufferByApostrophe();

    std::optional<std::string> bufferAsWord();
};
