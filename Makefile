CXXFLAGS=--std=c++20 -Wall -Wextra -Wpedantic -Werror -O0 -ggdb3
BUILD=build/
SRC=$(wildcard src/*.cpp)
OBJ=$(addprefix $(BUILD),$(notdir $(SRC:.cpp=.o)))
DEP=$(OBJ:.o=.d)
LIB=-lcpr -lfmt -lpugixml -ltidy
EXE=sim-file-audit

$(EXE): $(OBJ)
	$(CXX) $(LIB) -o $(EXE) $(OBJ)

$(BUILD)%.o: src/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@ -c $<

.PHONY: clean
clean:
	$(RM) $(OBJ) $(DEP) $(EXE)

.PHONY: force
force: clean $(EXE)

-include $(DEP)
