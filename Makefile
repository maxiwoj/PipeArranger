publish:
	g++ -E -nostdinc -Dinclude=#include -I. -Isystem-headers main.cpp | grep -v '^# [0-9]' >> asd.cpp
