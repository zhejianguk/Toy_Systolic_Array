# ---------- toolchain ----------
CXX      := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra

# ---------- project structure ----------
# Sources used by *every* test
COMMON_SRC := Matrix.cpp SystolicArray.cpp
COMMON_OBJ := $(COMMON_SRC:.cpp=.o)

# One .cpp file per test executable (e.g. Test_MatMual.cpp …)
TESTS      := Test_MatMul Test_WeightSationary
TEST_OBJS  := $(TESTS:%=%.o)          # produces Test_MatMual.o Test_WeightSationary.o
TARGETS    := $(TESTS)                # final executables

# ---------- default target ----------
.PHONY: all
all: $(TARGETS)

# ---------- link rule (executable ← its object + shared objects) ----------
# e.g. Test_MatMual: Test_MatMual.o Matrix.o SystolicArray.o
$(TESTS): %: %.o $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ---------- compile rule (object ← source) ----------
# catches both COMMON_SRC and TEST_OBJS
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------- housekeeping ----------
.PHONY: clean
clean:
	rm -f $(COMMON_OBJ) $(TEST_OBJS) $(TARGETS)
