CXXFLAGS=--std=c++20 -Wall -Wextra -Wpedantic -Werror -O0 -ggdb3
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(SRC:.cpp=.d)
LIB=-lcpr -lfmt -lpugixml -ltidy
EXE=sim-file-audit

$(EXE): $(OBJ)
	$(CXX) $(LIB) -o $(EXE) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@ -c $<

.PHONY: clean
clean:
	$(RM) $(OBJ) $(DEP) $(EXE)

.PHONY: force
force: clean $(EXE)

-include $(DEP)
