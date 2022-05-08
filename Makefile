# Main makefile of project
#
# @author Martin Kocich
# Contact: xkocic02@stud.fit.vutbr.cz

TARGET    ?= ICP
SRC_DIR   ?= ./src
INC_DIRS  ?= ./include
BUILD_DIR ?= ./build
DOC_DIR   ?= ./doc
RES_DIR   ?= ./resources

VARIANT   := 1
AUTHORS   := xkrizv03-xkocic02

PACK_FILES := $(DOC_DIR)/Doxyfile examples $(INC_DIRS) $(RES_DIR) $(SRC_DIR) CMakeLists.txt Makefile README.md README.txt resources.qrc windeployqt.cmake

# provides vars to all chid processes
export

run: build
	cd $(BUILD_DIR) && ./$(TARGET)

pack: clean
	zip -r $(VARIANT)-$(AUTHORS).zip $(PACK_FILES)

build:
	$(MAKE) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET):
	cd $(SRC_DIR) && $(MAKE)

doc:
	cd $(DOC_DIR) && doxygen Doxyfile

clean:
	cd $(SRC_DIR) && $(MAKE) clean
	cd $(DOC_DIR) && rm -rf html

.PHONY: run build doc clean
