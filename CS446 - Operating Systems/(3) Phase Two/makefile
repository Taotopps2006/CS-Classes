all: ConfigurationSettings.cpp Logger.cpp OperatingSystem.cpp PhaseTwo.cpp ProcessControlBlock.cpp SimpleTimer.cpp
	cd re2-master && make
	mv re2-master/obj/so/libre2.so.0 libre2.so.0
	g++ -std=c++11 -Wall -g ConfigurationSettings.cpp Logger.cpp OperatingSystem.cpp PhaseTwo.cpp ProcessControlBlock.cpp SimpleTimer.cpp -o sim02 -Wl,-rpath,'$ORIGIN' -l:libre2.so.0