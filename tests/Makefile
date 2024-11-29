run: tests
	./tests

tests: tests.cpp ard_utils
	c++ --std c++14 -I ../include -I ../../proto_activities/include -L . -lpa_ard_utils.o tests.cpp -o tests

ard_utils: ../include/pa_ard_utils.h
	c++ --std c++14 -I . -I ../include -I ../../proto_activities/include -c ../src/pa_ard_utils.cpp

clean:
	rm tests
	rm *.o
