#ifndef SENTIMENT_DICTIONARY_H
#define SENTIMENT_DICTIONARY_H

#include <unordered_map>
#include <string>

struct SentimentEntry
{
    float score;
    bool flip;
};

const std::unordered_map<std::string, SentimentEntry> sentimentDictionary{
    {"good", {3, false}},
    {"better", {2, false}},
    {"excellent", {5, false}},
    {"fantastic", {5, false}},
    {"great", {4, false}},
    {"wonderful", {5, false}},
    {"amazing", {5, false}},
    {"superb", {5, false}},
    {"outstanding", {5, false}},
    {"delicious", {5, false}},
    {"tasty", {4, false}},
    {"loved", {5, false}},
    {"perfect", {5, false}},
    {"brilliant", {5, false}},
    {"nice", {4, false}},
    {"satisfactory", {3, false}},
    {"satisfying", {4, false}},

    {"bad", {-3, false}},
    {"terrible", {-4, false}},
    {"awful", {-4, false}},
    {"horrible", {-5, false}},
    {"disappointing", {-3, false}},
    {"disappointed", {-3, false}},
    {"poor", {-3, false}},
    {"worst", {-5, false}},
    {"unpleasant", {-3, false}},
    {"bland", {-3, false}},
    {"unappetizing", {-4, false}},
    {"not", {-1.2, true}},

    {"really", {1.2, true}},
    {"very", {1.2, true}},
    {"quite", {1.1, true}},
};

#endif