#include <vector>
#include <sstream>
#include <random>
#include "board.h"

// I had a lot of performance concerns going in, but on my computer at least it takes
// several seconds just to read the file and then basically no time to process
// A board size of 100x100 is necessary for the two stages to be comparable
// Nothing fancy is needed to optimize the dictionary load time - storing a preprocessed
// dictionary is very realistic if it needed to be fast

// randomly generate a board 
Board::Board(unsigned sideLen) : gen(), dist(0, 25), sideLength(sideLen) {
  for(unsigned i = 0; i < sideLength; i++) {
    std::vector<char> v{};
    for(unsigned j = 0; j < sideLength; j++) {
      // adding 'A' like this isn't TECHNICALLY portable, but if someone is trying
      // to run something like this in a place where A-Z isn't consecutive...
      // well, that seems very unlikely.
      v.push_back(static_cast<char>(dist(gen) + 'A'));
    }
    boardVals.push_back(v);
  }
}
  
// the purpose of this board isn't to give dynamic access, just to store info
// and copying chars is extremely cheap, so this makes sense
// "at" is both a sensible name and a familiar one, and indicates that it's implemented
// in terms of the .at() of its underlying container and will therefore throw rather
// than producing UB on out-of-bounds accesses
char Board::at(unsigned i, unsigned j) const {
  char c = boardVals.at(i).at(j);
  // we want this to be adjusted since the mark's representation should be transparent
  // to the user
  if(marked(c)) return static_cast<char>(c + 'A');
  else return c;
}
  
bool Board::marked(unsigned i, unsigned j) const { return rawAt(i, j) < 'A'; }
// it's an unsigned
// if <0 comes in, it'll just wrap and fail the test as it should
bool Board::inbounds(unsigned i, unsigned j) const {
  return i < sideLength && j < sideLength;
}
unsigned Board::getSideLength() const { return sideLength; }
  
// a char is way bigger than we need so we can fiddle with the representation
// to mark one as visited; I think you mentioned this idea during the interview
// whatever the case, it's neat
// though possibly a premature optimization

// These two do nothing in the case that their desired effect is already present
// We also access by index afterward because we JUST did the bounds checking with
// .rawAt() so there's no need to do it twice
void Board::mark(unsigned i, unsigned j) {
  if(rawAt(i, j) >= 'A') boardVals[i][j] = static_cast<char>(boardVals[i][j] - 'A');
}
// Don't mix up the order! Capital is unmarked by my convention, so add capital A
// to unmark. I could have also done the difference between 'A' and 'a' but
// I really also could have done anything 26 or larger. 
void Board::unmark(unsigned i, unsigned j) {
  if(rawAt(i, j) < 'A') boardVals[i][j] = static_cast<char>(boardVals[i][j] + 'A');
}

// It's nice to have a string representation around.
// << isn't suitable everywhere. 
std::string Board::toString() const {
  std::ostringstream out;
  out << "Board: ";
  for(unsigned i = 0; i < getSideLength(); i++) {
    out << '\n';
    for(unsigned j = 0; j < getSideLength(); j++) {
      // useful for debugging
      if(!marked(i, j)) out << at(i, j);
      else out << static_cast<char>(tolower(at(i, j)));
    }
  }
  out << '\n';
  return out.str();
}

// Hey! This will give you the underlying data, which has to do with the implementation
// details of marking. Use .at() if that's not what you want.
char Board::rawAt(unsigned i, unsigned j) const { return boardVals.at(i).at(j); }
// takes "raw" values; only of interest internally
bool Board::marked(char c) { return c < 'A'; }

std::ostream& operator<<(std::ostream& out, const Board& board) {
  out << board.toString();
  return out;
}
