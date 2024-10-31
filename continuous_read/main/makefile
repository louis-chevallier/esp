
start :
	cp esp_timer_inc.h esp_timer.h
	c++ -g -Wno-format-security test.cpp
	valgrind  --leak-check=yes --tool=memcheck ./a.out
	-rm esp_timer.h

startx :
	cd ..; make cbuild



