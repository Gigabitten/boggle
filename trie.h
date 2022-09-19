#ifndef FIRSTTRIE
#define FIRSTTRIE

#include <unordered_map>
#include <memory>

class TrieNode {
public:
  explicit TrieNode(bool isEnd = false);
  // cat, category, and catastrophe all count, so we need this kind of info
  bool isWordEnd;
  // shared_ptr has overhead but makes a lot of sense here since we'll need to refer to it
  // in several places a lot, including keeping handles to it around
  // we could also optionally manage memory manually
  // and this is an optimization to take if shared_ptr turns out to be using a large
  // amount of runtime for one reason or another
  std::unordered_map<char, std::shared_ptr<TrieNode>> letterMap;
};

// We only need a few operations out of this:
// - insert a word
// - attempt to follow a letter deeper
class Trie {
public:
  using node_ptr = std::shared_ptr<TrieNode>;
  
  explicit Trie();

  // look up c
  // go deeper if it's found
  // if not, return nullptr as a sentinel
  static node_ptr dive(Trie::node_ptr node, char c);

  // The approach here is just to add letters unconditionally if they're not found.
  // Follow further either way.
  void addWord(std::string s);

  // A little awkward - works on node_ptrs, not tries.
  static std::string toString(node_ptr node, unsigned depth = 0);

  node_ptr root;
};

std::ostream& operator<<(std::ostream& os, const Trie::node_ptr node);

#endif
