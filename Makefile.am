# Makefile.am

bin_PROGRAMS = Wizard test1 test2

AM_CXXFLAGS = -Wall -std=c++11
AM_LDFLAGS = -lginac -lcln
COMMON_CPPFLAGS = -I. `pkg-config --cflags ginac` `pkg-config --cflags cln`

# Wizard
Wizard_SOURCES = main.cpp Wizard.hpp
Wizard_CPPFLAGS = $(COMMON_CPPFLAGS)
Wizard$(EXEEXT): Wizard-main.o
	$(CXX) $(CXXFLAGS) $^ $(AM_LDFLAGS) -o $@

# test1
test1_SOURCES = test1.cpp Wizard.hpp
test1_CPPFLAGS = $(COMMON_CPPFLAGS)
test1$(EXEEXT): test1-test1.o
	$(CXX) $(CXXFLAGS) $^ $(AM_LDFLAGS) -o $@

# test2
test2_SOURCES = test2.cpp Wizard.hpp
test2_CPPFLAGS = $(COMMON_CPPFLAGS)
test2$(EXEEXT): test2-test2.o
	$(CXX) $(CXXFLAGS) $^ $(AM_LDFLAGS) -o $@


