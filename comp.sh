compiler="g++"
sources="boggle.cpp board.cpp trie.cpp"
flags="-Wshadow -Wall -Wextra -Wconversion"
libflags=""
debugflags="-fsanitize=address -g -O0"

if [[ $# > 0 && $1 == "-t" ]]; then
    echo "building test"
    flags="${flags} ${debugflags}"
fi

eval $compiler $flags $libflags $sources
