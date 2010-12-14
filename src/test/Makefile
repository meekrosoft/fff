$(VERBOSE).SILENT:

C_PROGNAME = ../../build/cmock
CPP_PROGNAME = ../../build/cppmock
CC = gcc
CC += -c
CPP = g++
CPP += -c
LD = g++

ODIR = ../../build

C_OBJFILES = $(patsubst %.c,../../build/%.o,$(wildcard *.c))

CPP_OBJFILES = ../../build/cppmocktest.o ../../build/embedded.o
CPP_LIBS = -lgtest -lpthread -lgtest_main

ODIR = ../../build

all: $(C_PROGNAME) $(CPP_PROGNAME)

.PHONY: clean

clean:
	@echo "Cleaning object files"
	@echo "    rm -f     ../../build/*.o"
	rm -f ../../build/*.o
	@echo "Cleaning backups"
	@echo "    rm -f     *~"
	rm -f *~
	@echo "Removing programs"
	@echo "    rm -f     "$(C_PROGNAME)
	rm -f $(C_PROGNAME)
	@echo "    rm -f     "$(CPP_PROGNAME)
	rm -f $(CPP_PROGNAME)


../../build/%.o: %.c
	@echo "Compiling "$@
	@echo "    CC        "$<
	$(CC)  -o $@ $<

../../build/%.o: %.cpp
	@echo "Compiling "$@
	@echo "    CPP       "$<
	$(CPP) -I/home/mlong/tools/gtest/gtest-1.5.0/include -o $@ $< 

$(C_PROGNAME): $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(C_PROGNAME)"        "$(C_OBJFILES)
	$(LD) -o $(C_PROGNAME) $(C_OBJFILES)

$(CPP_PROGNAME): $(CPP_OBJFILES) $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(CPP_PROGNAME)"        "$(CPP_OBJFILES)
	$(LD) -L/home/mlong/tools/gtest -o $(CPP_PROGNAME) $(CPP_OBJFILES) $(CPP_LIBS)



nothing:
	@echo "Nothing to do; quitting  :("
	@echo "HINT: Try make all"
