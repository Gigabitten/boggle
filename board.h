#ifndef BOGGLEBOARD
#define BOGGLEBOARD

#include <vector>
#include <random>
#include <sstream>

class Board {
public:
  // logically 2-dimensional and square
  // vector gives us fast random access
  using board_container = std::vector<std::vector<char>>;
  
  // randomly generate a board 
  explicit Board(unsigned sideLen);
  
  // gives the char at (i,j)
  char at(unsigned i, unsigned j) const;
  
  // true if (i,j) is marked
  bool marked(unsigned i, unsigned j) const;
  
  // true if (i,j) is in the board's square
  bool inbounds(unsigned i, unsigned j) const;
  
  unsigned getSideLength() const;
  
  // does nothing if already marked
  void mark(unsigned i, unsigned j);
  
  // does nothing if already marked
  void unmark(unsigned i, unsigned j);
  
  // convert board to simple string, each line is a row
  std::string toString() const;

private:
  // Hey! This will give you the underlying data, which has to do with the implementation
  // details of marking. Use .at() if that's not what you want.
  char rawAt(unsigned i, unsigned j) const;
  
  // takes "raw" values; only of interest internally
  static bool marked(char c);
  
  // makes sense to have this sort of implementation detail be a private member
  std::default_random_engine gen;
  
  std::uniform_int_distribution<char> dist;
  
  unsigned sideLength;
  
  // as for this, we can easily streamline access
  board_container boardVals;
};

// logically belongs with the board
std::ostream& operator<<(std::ostream& out, const Board& board);

#endif
