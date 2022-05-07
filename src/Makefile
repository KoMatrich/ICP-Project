# Main makefile of project
#
# @author Martin Kocich
# Contact: xkocic02@stud.fit.vutbr.cz
#
# Literature used: https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_chapter/make_6.html

TARGET_EXEC ?= ICP.out

BUILD_DIR ?= ../build
SRC_DIRS ?= ./
INC_DIRS ?= ../include /usr/local/share/Qt-5.5.1/5.5/gcc_64/include

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(SRCS:./%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC := $(shell find $(INC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
