CXX = @cl.exe
CXXFLAGS = /c /EHsc /Ox \
                -I "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include" \
                -I "C:\Program Files\Microsoft SDKs\Windows\v7.1\Include"
LINK = @link.exe
LINKFLAGS = /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\amd64" \
            /LIBPATH:"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\x64" \
            /RELEASE
RC = @rc.exe
SOURCE = ..\Source
BINARY = .\


all: myginv.exe

myginv.exe: myginv.res myginv.obj version.obj involutive32_64.obj janettree32_64.obj poly32_64.obj monom32_64.obj variables.obj timer.obj
    @del $(BINARY)\*.exe 2> NUL
    $(LINK) $(LINKFLAGS) /OUT:$(BINARY)\myginv.exe \
            $(BINARY)\myginv.obj \
            $(BINARY)\version.obj \
            $(BINARY)\involutive32_64.obj \
            $(BINARY)\janettree32_64.obj \
            $(BINARY)\poly32_64.obj \
            $(BINARY)\monom32_64.obj \
            $(BINARY)\variables.obj \
            $(BINARY)\timer.obj 2> NUL
    @del $(BINARY)\*.obj
    @del $(BINARY)\*.res
    @echo ***** Building successfully completed *****

myginv.res: myginv.rc $(SOURCE)\version.h
    $(RC) /fomyginv.res myginv.rc 2> NUL

myginv.obj: $(SOURCE)\init32_64.h $(SOURCE)\variables.h $(SOURCE)\involutive32_64.h $(SOURCE)\version.h $(SOURCE)\timer.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\myginv.cpp /Fo$(BINARY)\myginv.obj 2> NUL

version.obj: $(SOURCE)\version.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\version.cpp /Fo$(BINARY)\version.obj 2> NUL

involutive32_64.obj: $(SOURCE)\involutive32_64.h $(SOURCE)\timer.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\involutive32_64.cpp /Fo$(BINARY)\involutive32_64.obj 2> NUL

janettree32_64.obj: $(SOURCE)\janettree32_64.h $(SOURCE)\poly32_64.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\janettree32_64.cpp /Fo$(BINARY)\janettree32_64.obj 2> NUL

poly32_64.obj: $(SOURCE)\poly32_64.h $(SOURCE)\monom32_64.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\poly32_64.cpp /Fo$(BINARY)\poly32_64.obj 2> NUL

monom32_64.obj: $(SOURCE)\monom32_64.h $(SOURCE)\variables.h $(SOURCE)\util.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\monom32_64.cpp /Fo$(BINARY)\monom32_64.obj 2> NUL

variables.obj: $(SOURCE)\variables.h $(SOURCE)\util.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\variables.cpp /Fo$(BINARY)\variables.obj 2> NUL

timer.obj: $(SOURCE)\timer.h
    $(CXX) $(CXXFLAGS) $(SOURCE)\timer.cpp /Fo $(BINARY)\timer.obj 2> NUL
