#include "text_processing.hpp"

#include <boost/algorithm/string.hpp>

#include "settings.hpp"

WordsStatistics::WordsStatistics()
    : uniqueWordsSequence(), wordsInSequence(), uniqueWords(), wordsCount(0) {}

void WordsStatistics::addWord(std::string word) {
    // string is taken by value because we later need transform letters to
    // lower case
    boost::algorithm::to_lower(word);
    while (wordsInSequence.count(word) > 0) {
        wordsInSequence.erase(uniqueWordsSequence.front());
        uniqueWordsSequence.pop_front();
    }
    uniqueWords.insert(word);
    wordsInSequence.insert(word);
    uniqueWordsSequence.emplace_back(std::move(word));
    ++wordsCount;
}

int64_t WordsStatistics::getWordsCount() { return wordsCount; }

const std::deque<std::string>& WordsStatistics::getUniqueWordsSequence() {
    return uniqueWordsSequence;
}

const std::unordered_set<std::string>& WordsStatistics::getUniqueWords() {
    return uniqueWords;
}

std::vector<std::string> WordsBufferedReader::readWords(const std::string& data,
                                                        bool noMoreData) {
    // we can treat non ascii characters as special symbols, as word by
    // provided definition is sequence of latin characters in utf-8
    std::vector<std::string> words;
    for (unsigned char c : data) {
        // check for isASCII in case of non default locale
        if (isASCII(c) && std::isalpha(c)) {
            buffer.push_back(c);
        } else if (isApostrophe(c) && !buffer.empty() &&
                   !isApostrophe(buffer.back())) {
            buffer.push_back(c);
        } else {
            auto optionalWord = bufferAsWord();
            if (optionalWord.has_value()) {
                words.emplace_back(std::move(optionalWord.value()));
            }
        }
        {
            auto optionalWord = splitBufferByApostrophe();
            if (optionalWord.has_value()) {
                words.emplace_back(std::move(optionalWord.value()));
            }
        }
        if (buffer.size() == MAX_WORD_LENGTH) {
            auto optionalWord = bufferAsWord();
            if (optionalWord.has_value()) {
                words.emplace_back(std::move(optionalWord.value()));
            }
        }
    }
    return words;
}

std::optional<std::string> WordsBufferedReader::splitBufferByApostrophe() {
    constexpr auto NUM_CHARS_INCLUDING_APOSTROPHE =
        NUM_CHARS_AFTER_APOSTROPHE + 1;
    if (buffer.size() > NUM_CHARS_INCLUDING_APOSTROPHE) {
        size_t NthIndexFromEnd =
            buffer.size() - NUM_CHARS_INCLUDING_APOSTROPHE - 1;
        if (isApostrophe(buffer[NthIndexFromEnd])) {
            std::string newBuff(buffer.begin() + NthIndexFromEnd + 1,
                                buffer.end());
            buffer.erase(buffer.begin() + NthIndexFromEnd, buffer.end());
            std::string word = std::move(buffer);
            buffer = std::move(newBuff);
            return word;
        }
    }
    return std::optional<std::string>();
}

std::optional<std::string> WordsBufferedReader::bufferAsWord() {
    boost::algorithm::trim_right_if(buffer, isApostrophe);
    if (!buffer.empty()) {
        std::string word = "";
        swap(word, buffer);
        return word;
    }
    return std::optional<std::string>();
}