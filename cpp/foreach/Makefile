# Add the BOOST PATH header files to CXXFLAGS
# Add the gtest library path to LDFLAGS
include ~/programming/make/Makefile

CXXFLAGS += -g

SRCS := foreachUnitTest.cpp \
	zipUnitTest.cpp \
	main.cpp

OBJS := $(SRCS:%.cpp=%.o)

%UnitTest.cpp: %.h
	touch $@

all: foreach.test

foreach.test: $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

