run: tests
	./tests

tests: tests.cpp ard_utils_base ard_utils
	c++ --std c++14 -I ../include -I ../../proto_activities/include -L . -lpa_ard_utils_base.o tests.cpp -o tests

ard_utils_base: ../include/pa_ard_utils_base.h ../include/priv/pa_ard_utils_base_priv.h ../src/pa_ard_utils_base.cpp
	c++ --std c++14 -I . -I ../include -I ../../proto_activities/include -c ../src/pa_ard_utils_base.cpp

ard_utils: ../include/pa_ard_utils.h ../include/priv/pa_ard_utils_priv.h ../src/pa_ard_utils.cpp
	c++ --std c++14 -I . -I ../include -I ../../proto_activities/include -c ../src/pa_ard_utils.cpp

clean:
	rm tests
	rm *.o
