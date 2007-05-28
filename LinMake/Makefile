CXX = g++
CXXFLAGS = -O3 -c
SOURCE = ../Source
BINARY = .

all: myginv

myginv: myginv.o igroebner64.o imypoly64.o imymonom64.o ivariables.o itimer.o
	$(CXX) \
            $(BINARY)/myginv.o \
            $(BINARY)/igroebner64.o \
            $(BINARY)/imypoly64.o \
            $(BINARY)/imymonom64.o \
            $(BINARY)/ivariables.o \
            $(BINARY)/itimer.o \
            -o $(BINARY)/myginv
	@rm $(BINARY)/*.o

myginv.o: $(SOURCE)/init64.h $(SOURCE)/ivariables.h $(SOURCE)/igroebner64.h $(SOURCE)/itimer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/myginv.cpp -o $(BINARY)/myginv.o

igroebner64.o: $(SOURCE)/igroebner64.h $(SOURCE)/imypoly64.h $(SOURCE)/itimer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/igroebner64.cpp -o $(BINARY)/igroebner64.o

imypoly64.o: $(SOURCE)/imypoly64.h $(SOURCE)/imymonom64.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/imypoly64.cpp -o $(BINARY)/imypoly64.o

imymonom64.o: $(SOURCE)/imymonom64.h $(SOURCE)/ivariables.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/imymonom64.cpp -o $(BINARY)/imymonom64.o

ivariables.o: $(SOURCE)/ivariables.h $(SOURCE)/iutil.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/ivariables.cpp -o $(BINARY)/ivariables.o

itimer.o: $(SOURCE)/itimer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/itimer.cpp  -o $(BINARY)/itimer.o
