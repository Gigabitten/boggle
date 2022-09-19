# boggle
Finds words in boggle. Quite quickly, in fact. Originally an interview question; I got the job, but it's a fun project which I decided to keep at a bit more. 

Needs a word list (wordlist.txt) for the executable to use. A word list is simply a whitespace-separated list of words. I used https://github.com/dwyl/english-words but anything can be used. ./comp.sh compiles; run with the -t flag for debugging symbols, ASAN, etc. Could have used a makefile, but that's a bit overkill here. 
