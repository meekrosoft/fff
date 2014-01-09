$(VERBOSE).SILENT:

BUILD_DIR = ../../build
TEMPLATE_PROGNAME = $(BUILD_DIR)/template
C_PROGNAME = $(BUILD_DIR)/ui_test_ansic
CPP_PROGNAME = $(BUILD_DIR)/ui_test_cpp
CC = gcc
CC += -c
CPP = g++
CPP += -c
LD = g++

GTEST_OBJS = $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest-main.o
C_OBJFILES = $(BUILD_DIR)/UI_test_ansic.o $(BUILD_DIR)/UI.o
TEMPLATE_OBJFILES = $(BUILD_DIR)/test_suite_template.o
CPP_OBJFILES = $(BUILD_DIR)/UI_test_cpp.o $(BUILD_DIR)/UI.o $(GTEST_OBJS)
CPP_LIBS = -lpthread 


all: $(C_PROGNAME) $(CPP_PROGNAME) $(TEMPLATE_PROGNAME)

.PHONY: clean

clean:
	@echo "Cleaning object files"
	@echo "    rm -f     $(BUILD_DIR)/*.o"
	rm -f $(BUILD_DIR)/*.o
	@echo "Cleaning backups"
	@echo "    rm -f     *~"
	rm -f *~
	@echo "Removing programs"
	@echo "    rm -f     "$(C_PROGNAME)
	rm -f $(C_PROGNAME)
	@echo "    rm -f     "$(CPP_PROGNAME) $(TEMPLATE_PROGNAME)
	rm -f $(CPP_PROGNAME) $(TEMPLATE_PROGNAME)


$(BUILD_DIR)/%.o: %.c
	@echo "Compiling "$@
	@echo "    CC        "$<
	$(CC)  -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling "$@
	@echo "    CPP       "$<
	$(CPP) -DGTEST_USE_OWN_TR1_TUPLE=1 -I../.. -o $@ $< 

$(TEMPLATE_PROGNAME): $(TEMPLATE_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "ctemplate"        "$(TEMPLATE_OBJFILES)
	$(LD) -o $(TEMPLATE_PROGNAME) $(TEMPLATE_OBJFILES)

$(C_PROGNAME): $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(C_PROGNAME)"        "$(C_OBJFILES)
	$(LD) -o $(C_PROGNAME) $(C_OBJFILES)

$(CPP_PROGNAME): $(CPP_OBJFILES) $(C_OBJFILES)
	@echo "Linking "$@
	@echo "    LD        -o "$(CPP_PROGNAME)"        "$(CPP_OBJFILES)
	$(LD) -o $(CPP_PROGNAME) $(CPP_OBJFILES) $(CPP_LIBS)



nothing:
	@echo "Nothing to do; quitting  :("
	@echo "HINT: Try make all"
