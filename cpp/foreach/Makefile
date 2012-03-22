# Add the BOOST PATH header files to CXXFLAGS
# Add the gtest library path to LDFLAGS

CXXFLAGS += -g

SRCS := ForeachUnitTest.cpp \
	main.cpp

OBJS := $(SRCS:%.cpp=%.o)

all: foreach.test

ForeachUnitTest.cpp: Foreach.h
	touch $@

foreach.test: $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(LDFLAGS)


