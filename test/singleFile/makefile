FPATH=../..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

CPP_FILES=$(notdir $(wildcard *.cpp))
EXE_FILES=$(CPP_FILES:.cpp=$(F_EXEC_EXT))
F_PROJ_MK=$(FPATH)/include/FRONTIER/Makefiles/fproject.mk

F_O_DIR=build

# for every cpp file call make w/ fproject.mk on it
all: $(EXE_FILES)

%$(F_EXEC_EXT): %.cpp
	$(MAKE) --no-print-directory -f $(F_PROJ_MK) F_CPP_FILES=$< EXEC=$(@:.exe=) FPATH=$(FPATH) CXX=$(CXX)

clean: clean_o clean_exec

clean_o:
	$(RRM) $(F_O_DIR)

clean_exec:
	$(RM) $(CPP_FILES:.cpp=$(F_EXEC_EXT))
