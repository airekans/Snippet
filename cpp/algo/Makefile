# Add the BOOST PATH header files to CXXFLAGS
# Add the gtest library path to LDFLAGS
CXXFLAGS += -g

SRCS := main.cpp

OBJS := $(SRCS:%.cpp=%.o)

%UnitTest.cpp: %.h
	touch $@

all: BigInt.test

BigInt.test: $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

