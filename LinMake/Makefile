CXX = g++
CXXFLAGS = -O3 -c
SOURCE = ../Source
BINARY = .

all: myginv

myginv: myginv.o igroebner64.o poly64.o monom64.o iallocator.o ivariables.o itimer.o
	$(CXX) \
            $(BINARY)/myginv.o \
            $(BINARY)/igroebner64.o \
            $(BINARY)/poly64.o \
            $(BINARY)/monom64.o \
            $(BINARY)/iallocator.o \
            $(BINARY)/ivariables.o \
            $(BINARY)/itimer.o \
            -o $(BINARY)/myginv
	@rm $(BINARY)/*.o

myginv.o: $(SOURCE)/init64.h $(SOURCE)/ivariables.h $(SOURCE)/igroebner64.h $(SOURCE)/itimer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/myginv.cpp -o $(BINARY)/myginv.o

igroebner64.o: $(SOURCE)/igroebner64.h $(SOURCE)/poly64.h $(SOURCE)/itimer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/igroebner64.cpp -o $(BINARY)/igroebner64.o

poly64.o: $(SOURCE)/poly64.h $(SOURCE)/monom64.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/poly64.cpp -o $(BINARY)/poly64.o

monom64.o: $(SOURCE)/monom64.h $(SOURCE)/ivariables.h $(SOURCE)/iallocator.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/monom64.cpp -o $(BINARY)/monom64.o

iallocator.o: $(SOURCE)/iallocator.h $(SOURCE)/iutil.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/iallocator.cpp -o $(BINARY)/iallocator.o

ivariables.o: $(SOURCE)/ivariables.h $(SOURCE)/iutil.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/ivariables.cpp -o $(BINARY)/ivariables.o

itimer.o: $(SOURCE)/itimer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/itimer.cpp  -o $(BINARY)/itimer.o
