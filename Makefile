# Main makefile of project
#
# @author Martin Kocich
# Contact: xkocic02@stud.fit.vutbr.cz

SRC_DIR  ?= ./src
INC_DIRS ?= ./include
DOC_DIR  ?= ./doc
RES_DIR  ?= ./resources

PACK_FILES := $(DOC_DIR)/Doxyfile examples $(INC_DIRS) $(RES_DIR) $(SRC_DIR) CMakeLists.txt Makefile README.md resources.qrc windeployqt.cmake

run: build

pack:
	zip -r 1-xkocic02-xkrizv03.zip $(PACK_FILES)

build:
	cd $(SRC_DIR) && $(MAKE)

doc:
	cd $(DOC_DIR) && doxygen Doxyfile

clean:
	cd $(SRC_DIR) && $(MAKE) clean
	cd $(DOC_DIR) && rm -rf html

.PHONY: run doc clean
