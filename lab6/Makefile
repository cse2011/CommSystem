CXXFLAGS = -O3 -std=c++11
#CXXFLAGS = -g -std=c++11
CXX = g++

EXEC = audio exp

CSRCS=$(wildcard *.cpp)
COBJS  = $(addsuffix .o, $(basename $(CSRCS)))

audio : modem.o convolution.o util.o audio.o
	@echo "> Building $@"
	@$(CXX) $(LDFLAGS) $^ -o $@
exp : modem.o convolution.o util.o main.o
	@echo "> Building $@"
	@$(CXX) $(LDFLAGS) $^ -o $@
%.o: %.cpp
	@echo "> Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	@$(RM) EXEC
	@$(RM) output/*
	@$(RM) *.o
