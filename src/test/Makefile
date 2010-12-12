$(VERBOSE).SILENT:

C_PROGNAME = cmock
CPP_PROGNAME = cppmock
CC = gcc
CC += -c
CPP = g++
CPP += -c
LD = g++

C_OBJFILES = $(patsubst %.c,%.o,$(wildcard *.c))
CPP_OBJFILES = cppmocktest.o embedded.o


all: $(C_PROGNAME) $(CPP_PROGNAME)

clean:
	@echo "Cleaning object files"
	@echo "    rm -f     *.o"
	rm -f *.o
	@echo "Cleaning backups"
	@echo "    rm -f     *~"
	rm -f *~
	@echo "Removing programm"
	@echo "    rm -f     "$(C_PROGNAME)
	rm -f $(C_PROGNAME)
	@echo "    rm -f     "$(CPP_PROGNAME)
	rm -f $(CPP_PROGNAME)


%.o: %.c
	@echo "Compiling "$@
	@echo "    CC        "$<
	$(CC) $<

%.o: %.cpp
	@echo "Compiling "$@
	@echo "    CPP       "$<
	$(CPP) -I/home/mlong/tools/gtest/gtest-1.5.0/include $<

$(C_PROGNAME): $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(C_PROGNAME)"        "$(C_OBJFILES)
	$(LD) -o $(C_PROGNAME) $(C_OBJFILES)

$(CPP_PROGNAME): $(CPP_OBJFILES) $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(CPP_PROGNAME)"        "$(CPP_OBJFILES)
	$(LD) -L/home/mlong/tools/gtest -o $(CPP_PROGNAME) $(CPP_OBJFILES) -lgtest -lpthread -lgtest_main

nothing:
	@echo "Nothing to do; quitting  :("
	@echo "HINT: Try make all"
