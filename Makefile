publish:
	g++ -E -nostdinc -Dinclude=#include -D NDEBUG -I. -Isystem-headers main.cpp | grep -v '^# [0-9]' > asd.cpp
	echo '\n' >> asd.cpp
	cat defs.cpp >> asd.cpp
	echo '\n' >> asd.cpp
	cat Land.cpp >> asd.cpp
	sed 's/#include "/ \/\/ /g' asd.cpp > whole.cpp
	rm asd.cpp
	echo '#include <algorithm>' | cat - whole.cpp > temp && mv temp whole.cpp

compile:
	g++ -O2 -Wall -std=c++0x -std=gnu++0x whole.cpp

clean:
	rm *.out whole.cpp

testOutput:
	cp examples/$(TEST_CASE) judge/in
	cat examples/$(TEST_CASE) | ./a.out > judge/out
	cd judge ; python3 judge.py in out
	rm judge/in
	rm judge/out
