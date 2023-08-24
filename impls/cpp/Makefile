CXXFLAGS ?= -g -Wall -Wextra

step0_repl: step0_repl.cpp
	$(CXX) $(CXXFLAGS) -o step0_repl step0_repl.cpp 

step1_read_print: step1_read_print.cpp
	$(CXX) $(CXXFLAGS) -o step1_read_print step1_read_print.cpp types.cpp reader.cpp printer.cpp

step2_eval: step2_eval.cpp
	$(CXX) $(CXXFLAGS) -o step2_eval step2_eval.cpp types.cpp reader.cpp printer.cpp
