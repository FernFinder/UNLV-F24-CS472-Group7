// Code to be refactored for ChatGPT lab
// Kamil

#include <iostream>
#include <unordered_map>
#include <string>

void searchSubstrings(std::unordered_map<std::string, int> &foundSubstrings, std::unordered_map<std::string, int> wordList,
                      std::string &targetString, int startPosition, int wordLength, std::string currentSubstring);
// Displays all found substrings and their counts
void displaySubstrings(const std::unordered_map<std::string, int> &substringCounts);

int main()
{
    // Data structures to store word frequency and substring counts
    std::unordered_map<std::string, int> wordFrequencyMap;
    std::unordered_map<std::string, int> substringCountMap;

    // Input target string
    std::cout << "Enter target string: ";
    std::string targetString;
    std::cin >> targetString;

    // Input list of words
    std::cout << "Enter list of words (type 'END' to finish):\n";
    std::string inputWord;
    std::cin >> inputWord;

    // "END" signifies the end of the list (not included in list)
    while (inputWord != "END")
    {
        wordFrequencyMap[inputWord]++;
        std::cin >> inputWord;
    }

    int wordLength = wordFrequencyMap.begin()->first.size();
    std::cout << "List of substrings :\n";
    // Search for substrings
    while (targetString.size() >= wordLength)
    {
        // Iterate through the word frequency map
        for (auto mapIter = wordFrequencyMap.begin(); mapIter != wordFrequencyMap.end(); ++mapIter)
        {
            // Find the position of the word in the target string
            auto foundPosition = targetString.find(mapIter->first);
            if (foundPosition != std::string::npos)
                searchSubstrings(substringCountMap, wordFrequencyMap, targetString, foundPosition, wordLength, mapIter->first);
        }
        targetString.erase(0, wordLength);
    }
    // Display the found substrings
    if (substringCountMap.empty())
        std::cout << "no solution\n";
    else
        displaySubstrings(substringCountMap);
    return 0;
}

// Recursive function to search for substrings
void searchSubstrings(std::unordered_map<std::string, int> &substringCounts, std::unordered_map<std::string, int> wordList,
                      std::string &targetString, int index, int wordLength, std::string currentSubstring)
{
    std::string foundWord = targetString.substr(index, wordLength);
    wordList.erase(foundWord);
    // Update the index
    index += wordLength;
    // Update the current substring
    if (wordList.empty())
    {
        substringCounts[currentSubstring]++;
        return;
    }
    // Find the next word
    std::string nextWord = targetString.substr(index, wordLength);
    for (auto listIter = wordList.begin(); listIter != wordList.end(); ++listIter)
    {
        if (nextWord == listIter->first)
            searchSubstrings(substringCounts, wordList, targetString, index, wordLength, currentSubstring + nextWord);
    }
}

/**
 * @brief Prints substrings and their counts from a map.
 * 
 * Iterates over an unordered_map, `substringCounts`, printing each key-value pair where the key is a substring
 * and the value is its occurrence count. Output format is "<substring> - Count: <count>". The iteration order
 * is based on the map's internal hash table, hence unordered.
 * 
 * @param substringCounts An unordered_map with std::string keys (substrings) and int values (counts). Assumes
 *        the map is non-empty; an empty map results in no output.
 * 
 * @note This function produces output to std::cout and modifies no inputs.
 */
void displaySubstrings(const std::unordered_map<std::string, int> &substringCounts)
{
    for (const auto &pair : substringCounts)
        std::cout << pair.first << " - Count: " << pair.second << '\n';
}
