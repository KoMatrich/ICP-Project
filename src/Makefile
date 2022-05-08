# Main makefile of project
#
# @author Martin Kocich
# Contact: xkocic02@stud.fit.vutbr.cz
#
# Literature used: https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_chapter/make_6.html

# defautl build dir
BUILD_DIR ?= ./build

build:
	cd .. && mkdir -p $(BUILD_DIR)
	cd .. && cmake -B $(BUILD_DIR)
	cd .. && cd $(BUILD_DIR) && make

.PHONY: clean
clean:
	cd .. && $(RM) -r $(BUILD_DIR)
