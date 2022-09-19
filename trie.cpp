#include <unordered_map>
#include <memory>
#include <algorithm>
#include <sstream>
#include <utility>
#include "trie.h"

// I happen to know the name of this, but I'd like to take a crack at making it myself
// because it sounds kinda fun, and in the spirit of an interview question
// if it's horrifically slow, I'll look up how others did it

// we don't need this to be completely general
// we need an n-tree of sets which indicate whether a word has been formed
// normal tree problems don't even apply! no need for balancing and whatnot
TrieNode::TrieNode(bool isEnd /*= false*/)
  : isWordEnd(isEnd), letterMap() {}

Trie::Trie() : root(std::make_shared<TrieNode>()) { }

// look up c
// go deeper if it's found
// if not, return nullptr as a sentinel
Trie::node_ptr Trie::dive(Trie::node_ptr node, char c) {
  auto charLoc = node->letterMap.find(c);
  if(charLoc == node->letterMap.end()) return nullptr;
  else return charLoc->second;
}

// The approach here is just to add letters unconditionally if they're not found.
// Follow further either way.
void Trie::addWord(std::string s) {
  node_ptr curr = root;
  // for_each can often be replaced with a better algorithm, but sometimes the versatility
  // of an arbitrary side effect for every item in a group is hard to beat
  // this is one of those times
  std::for_each(s.cbegin(), s.cend(), [&curr](char c){
    // toupper returns an int, so the explicit cast gets -Wconversion to quiet down
    // it's loud, but on occasion saves me from something silly yet subtle
    c = static_cast<char>(toupper(c));
    // and here's the reason this has to be for_each
    // we don't even know where we're going until we get here
    auto& m = curr->letterMap;
    // We need to be careful not to use the add-or-update syntax carelessly
    // Here, a check precedes it; it's really just an insert
    if(m.find(c) == m.end()) m[c] = std::make_shared<TrieNode>();
    // we can find with no fear since we just inserted it
    curr = m.find(c)->second;
  });
  // whatever curr ends up at, that's definitely a word end
  // since we just... finished up a word.
  curr->isWordEnd = true;
}

// A little awkward - works on node_ptrs, not tries.
std::string Trie::toString(node_ptr node, unsigned depth /*= 0*/) {
  std::ostringstream s;
  for(unsigned i = 0; i < depth; i++) s << " ";
  s << "Is word end: " << node->isWordEnd << '\n';
  for(unsigned i = 0; i < depth; i++) s << " ";
  s << "Contains: ";
  depth += 1;
  std::for_each(node->letterMap.begin(), node->letterMap.end(), [&node, &s, depth](std::pair<const char, node_ptr>& n){
    for(unsigned i = 0; i < depth; i++) s << " ";
    s << n.first << '\n';
    s << toString(n.second, depth);
  });
  return s.str();
}

std::ostream& operator<<(std::ostream& os, const Trie::node_ptr node) {
  return os << Trie::toString(node);
}
