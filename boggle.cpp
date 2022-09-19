/* TODO(Caleb)
 * get performance profiling up and running
 * investigate changing unordered map to fixed array
 * investigate multithreading
 */
// Note the above exclusion of simplifying marking; though you could handle
// marking on the stack during the recursion in wordSearch, this makes it
// difficult to make a sensible toString for the board which integrates
// mark information. This functionality is very useful for debugging. 

#include <iostream>
#include <iterator>
#include <ostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <cassert>
#include "board.h"
#include "trie.h"

// For a first approach, a recursive depth-first search will be used.
// This makes managing state a piece of cake compared using an explicit stack.
// Sure it's a lot of recursion, but the maximum depth is only the length of the
// longest part of a word it looks for. Space isn't a big deal here; time may be.

// Remarkably simple algorithm, but a pain to debug. I accidentally put node
// instead of newNode in the recursive call! Too many small things can
// go wrong here. Took a while to spot the problem.
void wordSearch(const std::string& wordAcc, Board& board, const Trie::node_ptr node,
                unsigned i, unsigned j,
                std::vector<std::string>& finds) {
  board.mark(i, j);
  for(int iOffset = -1; iOffset <= 1; iOffset++) {
    for(int jOffset = -1; jOffset <= 1; jOffset++) {
      const unsigned currI = i + iOffset;
      const unsigned currJ = j + jOffset;
      if(board.inbounds(currI, currJ) && !board.marked(currI, currJ)) {
        const char charHere = board.at(currI, currJ);
        const Trie::node_ptr newNode = Trie::dive(node, charHere);
        if(newNode != nullptr) {
          // makes sense to make a copy
          // don't want to risk mutation problems as we loop
          const std::string currWord = wordAcc + charHere;
          if(newNode->isWordEnd) finds.push_back(currWord);
          wordSearch(currWord, board, newNode, currI, currJ, finds);
        }
      }
    }
  }
  // Can't forget to unmark!
  board.unmark(i, j);
}

std::vector<std::string> boardSearch(Board& board, Trie& trie) {
  std::vector<std::string> finds{};
  for(unsigned i = 0; i < board.getSideLength(); i++) {
    for(unsigned j = 0; j < board.getSideLength(); j++) {
      // The last screwy thing - we need to start somewhere, but really we need to start
      // sideLength^2 somewheres! More (mercifully minor) manual management of marking is mandatory.
      auto h = trie.root->letterMap.find(board.at(i, j));
      board.mark(i, j);
      if(h != trie.root->letterMap.end()) wordSearch(std::string{board.at(i, j)}, board, h->second, i, j, finds);
      board.unmark(i, j);      
    }
  }
  // We probably got a lot of duplicates. No need for those.
  std::sort(finds.begin(), finds.end());
  finds.erase(std::unique(finds.begin(), finds.end()),
              finds.end());
  // this vector can be quite large, but RVO should save us here
  return finds;
}

// we want this to be optional, but we also want to not have to regenerate the trie in particular
void assertPile(Trie& trie) {
  Board board{5};
  auto finds = boardSearch(board, trie);
  // the generator is unseeded so that we can test it. Here's what it gives:
  
  // ADTLN
  // FBRRY
  // JNVAB
  // NRAJB
  // KRPYW
  assert(std::binary_search(finds.begin(), finds.end(), "DRABBY"));
  // This is a funky one - it's in the word list, and you can almost make it, but not quite
  // it's a good test due to its structure of whether the marking is working right
  assert(!std::binary_search(finds.begin(), finds.end(), "RAVANA"));
  // and of course the big pile of more isolated asserts
  assert(!board.marked(3, 4));
  board.mark(3, 4);
  assert(board.marked(3, 4));
  board.mark(3, 4);
  assert(board.marked(3, 4));
  board.unmark(3, 4);
  assert(!board.marked(3, 4));
  board.unmark(3, 4);
  assert(!board.marked(3, 4));
  auto len = board.getSideLength();
  assert(board.inbounds(len - 1, len - 1));
  assert(!board.inbounds(len, len));
  // trie testing
  assert(!trie.root->isWordEnd);
  assert(trie.root->letterMap.find('C')->second->letterMap.find('A')->second->letterMap.find('T')->second->isWordEnd);
  assert(Trie::dive(Trie::dive(Trie::dive(trie.root, 'C'), 'A'), 'T')->isWordEnd);
  // checking the rules of the game (words must be 3 or more letters)
  assert(!Trie::dive(Trie::dive(trie.root, 'C'), 'A')->isWordEnd);
  // and that words are correctly marked as not ends
  assert(!Trie::dive(Trie::dive(Trie::dive(Trie::dive(trie.root, 'C'), 'A'), 'T'), 'W')->isWordEnd);
  // "vq" doesn't appear anywhere for any reason, and I can hardly think of an exception
  // that a different word list might include
  assert(Trie::dive(Trie::dive(trie.root, 'V'), 'Q') == nullptr);
  std::cout << "Assertions passed!" << std::endl;
}

int main() {
  // words from https://github.com/dwyl/english-words
  std::ifstream words{"wordlist.txt"};
  Board board{5};
  Trie trie{};
  std::string s;
  while(words >> s) {
    if(s.length() >= 3) {
      trie.addWord(s);
    }
  }
  // DON'T UNCOMMENT THIS WITH A BIG DICTIONARY
  // seriously, it's really big
  // std::cout << trie.root;
  std::cout << board;
  auto finds = boardSearch(board, trie);
  std::cout << finds.size() << " finds!" << '\n';
  std::copy(finds.begin(), finds.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
  assertPile(trie);
  return 0;
}
