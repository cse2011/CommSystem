CXXFLAGS = -O3 -std=c++11
CXX = g++

audio : audio.cpp
	@$(CXX) $(CXXFLAGS) $^ -o $@
exp : main.cpp
	@$(CXX) $(CXXFLAGS) $^ -o $@
clean:
	@$(RM) audio exp
	@$(RM) output/*
