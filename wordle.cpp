/*#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

}
*/
/*
// Define any helper functions here
#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

static bool containsFloating(const std::string& word,
                      const std::string& floating);

static void wordleHelper(std::string& cur,
                  int pos,
                  const std::string& floating,
                  const std::set<std::string>& dict,
                  std::set<std::string>& answers);
// Helper: check whether `word` contains all letters in `floating`
// with the correct multiplicity.
static bool containsFloating(const string& word, const string& floating)
{
    string temp = word;

    for (char f : floating) {
        bool found = false;
        for (size_t i = 0; i < temp.size(); i++) {
            if (temp[i] == f) {
                temp[i] = '#';   // mark this occurrence as used
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true;
}

// Helper: recursively build all candidate words
static void wordleHelper(string& cur,
                         int pos,
                         const string& floating,
                         const set<string>& dict,
                         set<string>& answers)
{
    if (pos == (int)cur.size()) {
        if (dict.find(cur) != dict.end() && containsFloating(cur, floating)) {
            answers.insert(cur);
        }
        return;
    }

    if (cur[pos] != '-') {
        wordleHelper(cur, pos + 1, floating, dict, answers);
        return;
    }

    for (char c = 'a'; c <= 'z'; c++) {
        cur[pos] = c;
        wordleHelper(cur, pos + 1, floating, dict, answers);
    }

    cur[pos] = '-';  // restore for backtracking
}

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> answers;
    string cur = in;
    wordleHelper(cur, 0, floating, dict, answers);
    return answers;
}*/
#include <set>
#include <string>
#include <vector>

#include "wordle.h"
#include "dict-eng.h"

using namespace std;

static void wordleHelper(string& cur,
                         int pos,
                         int blanksLeft,
                         vector<int>& floatingCount,
                         int floatingLeft,
                         const set<string>& dict,
                         set<string>& answers)
{
    if (floatingLeft > blanksLeft) {
        return;
    }

    if (pos == static_cast<int>(cur.size())) {
        if (floatingLeft == 0 && dict.find(cur) != dict.end()) {
            answers.insert(cur);
        }
        return;
    }

    if (cur[pos] != '-') {
        wordleHelper(cur, pos + 1, blanksLeft, floatingCount, floatingLeft, dict, answers);
        return;
    }

    const bool mustUseFloating = (floatingLeft == blanksLeft);

    for (char c = 'a'; c <= 'z'; ++c) {
        int idx = c - 'a';

        if (mustUseFloating && floatingCount[idx] == 0) {
            continue;
        }

        cur[pos] = c;

        bool usedFloating = false;
        if (floatingCount[idx] > 0) {
            floatingCount[idx]--;
            usedFloating = true;
        }

        wordleHelper(cur,
                     pos + 1,
                     blanksLeft - 1,
                     floatingCount,
                     floatingLeft - (usedFloating ? 1 : 0),
                     dict,
                     answers);

        if (usedFloating) {
            floatingCount[idx]++;
        }
    }

    cur[pos] = '-';
}

set<string> wordle(const string& in,
                   const string& floating,
                   const set<string>& dict)
{
    set<string> answers;
    string cur = in;

    vector<int> floatingCount(26, 0);
    for (char c : floating) {
        floatingCount[c - 'a']++;
    }

    int blanksLeft = 0;
    for (char c : in) {
        if (c == '-') {
            blanksLeft++;
        }
    }

    wordleHelper(cur, 0, blanksLeft, floatingCount, static_cast<int>(floating.size()), dict, answers);
    return answers;
}