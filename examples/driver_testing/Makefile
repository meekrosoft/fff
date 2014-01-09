$(VERBOSE).SILENT:

BUILD_DIR = ../../build
TEMPLATE_PROGNAME = $(BUILD_DIR)/template
CPP_PROGNAME_NOFFF = $(BUILD_DIR)/driver_testing
CPP_PROGNAME_FFF = $(BUILD_DIR)/driver_testing_fff
CC = gcc
CC += -c
CPP = g++
CPP += -c
LD = g++

GTEST_OBJS = $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest-main.o
C_OBJFILES = $(BUILD_DIR)/driver.o
TEMPLATE_OBJFILES = $(BUILD_DIR)/test_suite_template.o
FFF_OBJFILES = $(BUILD_DIR)/driver.test.fff.o $(GTEST_OBJS)
NOFFF_OBJFILES = $(BUILD_DIR)/driver.test.o $(GTEST_OBJS)
CPP_LIBS = -lpthread 


all: $(CPP_PROGNAME_NOFFF) $(CPP_PROGNAME_FFF) $(TEMPLATE_PROGNAME)

.PHONY: clean

clean:
	@echo "Cleaning object files"
	@echo "    rm -f     $(BUILD_DIR)/*.o"
	rm -f $(BUILD_DIR)/*.o
	@echo "Cleaning backups"
	@echo "    rm -f     *~"
	rm -f *~
	@echo "Removing programs"
	@echo "    rm -f     $(CPP_PROGNAME_NOFFF) $(CPP_PROGNAME_FFF) $(TEMPLATE_PROGNAME)"
	rm -f $(CPP_PROGNAME_NOFFF) $(CPP_PROGNAME_FFF) $(TEMPLATE_PROGNAME)


$(BUILD_DIR)/%.o: %.c
	@echo "Compiling "$@
	@echo "    CC        "$<
	$(CC)  -o $@ $< -DTESTING

$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling "$@
	@echo "    CPP       "$<
	$(CPP) -DGTEST_USE_OWN_TR1_TUPLE=1 -I../.. -o $@ $< -DTESTING

$(TEMPLATE_PROGNAME): $(TEMPLATE_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "ctemplate"        "$(TEMPLATE_OBJFILES)
	$(LD) -o $(TEMPLATE_PROGNAME) $(TEMPLATE_OBJFILES)

$(CPP_PROGNAME_FFF): $(FFF_OBJFILES) $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(CPP_PROGNAME_FFF)"        "$(FFF_OBJFILES)
	$(LD) -o $(CPP_PROGNAME_FFF) $(FFF_OBJFILES) $(C_OBJFILES) $(CPP_LIBS)

$(CPP_PROGNAME_NOFFF): $(NOFFF_OBJFILES) $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(CPP_PROGNAME_NOFFF)"        "$(NOFFF_OBJFILES)
	$(LD) -o $(CPP_PROGNAME_NOFFF) $(NOFFF_OBJFILES) $(C_OBJFILES) $(CPP_LIBS)

nothing:
	@echo "Nothing to do; quitting  :("
	@echo "HINT: Try make all"
