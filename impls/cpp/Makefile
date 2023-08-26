CXXFLAGS ?= -g -Wall -Wextra

step0_repl: step0_repl.cpp
	$(CXX) $(CXXFLAGS) -o step0_repl step0_repl.cpp 

step1_read_print: step1_read_print.cpp
	$(CXX) $(CXXFLAGS) -o step1_read_print step1_read_print.cpp types.cpp reader.cpp printer.cpp

step2_eval: step2_eval.cpp types.cpp reader.cpp printer.cpp env.cpp
	$(CXX) $(CXXFLAGS) -o step2_eval step2_eval.cpp types.cpp reader.cpp printer.cpp env.cpp

step3_env: step3_env.cpp types.cpp reader.cpp printer.cpp env.cpp
	$(CXX) $(CXXFLAGS) -o step3_env step3_env.cpp types.cpp reader.cpp printer.cpp env.cpp

step4_if_fn_do: step4_if_fn_do.cpp types.cpp reader.cpp printer.cpp env.cpp core.cpp
	$(CXX) $(CXXFLAGS) -o step4_if_fn_do step4_if_fn_do.cpp types.cpp reader.cpp printer.cpp env.cpp core.cpp

step5_tco: step5_tco.cpp types.cpp reader.cpp printer.cpp env.cpp core.cpp
	$(CXX) $(CXXFLAGS) -o step5_tco step5_tco.cpp types.cpp reader.cpp printer.cpp env.cpp core.cpp
