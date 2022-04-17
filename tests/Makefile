TARGET = crc_test

# Specify "yes" if you want to use C++ assembly otherwise C will be used
USE_CXX = yes



# Utils
RM             = rm -f
RM_RECURSIVE   = rm -rf
MKDIR          = mkdir -p
ECHO           = echo



# Toolchain 
TOOLCHAIN ?=riscv32-unknown-elf-

CC         =$(TOOLCHAIN)gcc
CXX        =$(TOOLCHAIN)g++
AS         =$(TOOLCHAIN)as
LD         =$(TOOLCHAIN)ld
OBJDUMP    =$(TOOLCHAIN)objdump
OBJCOPY    =$(TOOLCHAIN)objcopy
SIZE       =$(TOOLCHAIN)size
NM         =$(TOOLCHAIN)nm



# Dirs
BUILD_DIR        = ./build
RELEASE_DIR      = $(BUILD_DIR)/release
DEBUG_DIR        = $(BUILD_DIR)/debug



# Subdirs src
SUBDIRS_SRC      = ./



# Subdirs inc
SUBDIRS_INC      = ./ \
                   ../



I_INCLUDE = $(addprefix -I, $(SUBDIRS_INC))



SRC_TEMPLATE = $(addsuffix *.S, $(SUBDIRS_SRC)) \
               $(addsuffix *.c, $(SUBDIRS_SRC)) \
               $(addsuffix *.cpp, $(SUBDIRS_SRC))



SOURCES = $(wildcard $(SRC_TEMPLATE))



# Objects
OBJECTS  := $(patsubst %.S, %.o, $(SOURCES) )
OBJECTS  := $(patsubst %.c, %.o, $(OBJECTS) )
OBJECTS  := $(patsubst %.cpp, %.o, $(OBJECTS) )



# Objects debug
DEBUG_SUFFIX   = debug

DEBUG_OBJECTS := $(patsubst %.o, %_$(DEBUG_SUFFIX).o, $(OBJECTS))



# Depend
DEPENDENC  := $(patsubst %.o, %.d, $(OBJECTS) )

DEBUG_DEPENDENC  := $(patsubst %.o, %_$(DEBUG_SUFFIX).d, $(OBJECTS))



# Flags
ARCH_FLAGS   = -march=rv32i
ARCH_FLAGS  += -mabi=ilp32

OBJDUMP_M    =riscv

SPECS        = --specs=nano.specs
SPECS       += --specs=nosys.specs

DEBUG_FLAGS  =
DEBUG_FLAGS += -DDEBUG -g2

LDFLAGS      = -T link.ld
LDFLAGS     += -nostartfiles
# LDFLAGS     += -nostdlib
LDFLAGS     += -Wl,-lm
LDFLAGS     += -Wl,--start-group
LDFLAGS     += -Wl,--end-group
LDFLAGS     += -Wl,--gc-sections
LDFLAGS     += -Wl,--print-memory-usage
LDFLAGS     += -Wl,--cref
LDFLAGS     += $(ARCH_FLAGS)

CXXFLAGS    = -O2
CXXFLAGS    += -std=c++14
CXXFLAGS    += -s 
CXXFLAGS    += -Wall 
CXXFLAGS    += -Wextra
CXXFLAGS    += -pedantic
CXXFLAGS    += -fno-exceptions
CXXFLAGS    += -ffreestanding
CXXFLAGS    += -ffunction-sections
CXXFLAGS    += -fdata-sections
CXXFLAGS    += -fno-rtti
CXXFLAGS    += -fno-threadsafe-statics
CXXFLAGS    += -fno-use-cxa-atexit
CXXFLAGS    += $(ARCH_FLAGS)
CXXFLAGS    += $(SPECS)
CXXFLAGS    += -Wstack-usage=131072
CXXFLAGS    += $(I_INCLUDE)

CCFLAGS     = -O2
CCFLAGS     += -std=gnu11
CCFLAGS     += -Wall 
CCFLAGS     += -Wextra
CCFLAGS     += -pedantic
CCFLAGS     += -ffreestanding
CCFLAGS     += -ffunction-sections
CCFLAGS     += -fdata-sections
CCFLAGS     += $(ARCH_FLAGS)
CCFLAGS     += $(SPECS)
CCFLAGS     += -Wstack-usage=1000
CCFLAGS     += $(I_INCLUDE)



# Rules
.PHONY: all
all: debug release
.DEFAULT_GOAL := all



.PHONY: release
release: $(TARGET)



.PHONY: debug
debug: $(TARGET)_$(DEBUG_SUFFIX)



$(TARGET): CCFLAGS := -s $(CCFLAGS)
$(TARGET): CXXFLAGS := -s $(CXXFLAGS)
$(TARGET): $(OBJECTS)
	$(call build_elf, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_bin, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_hex, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_asm, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_dmp, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_sym, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_print_size, $(OBJECTS),$(RELEASE_DIR))
	$(call generate_hex_dump, $(RELEASE_DIR))



$(TARGET)_$(DEBUG_SUFFIX): CCFLAGS := $(DEBUG_FLAGS) $(CCFLAGS)
$(TARGET)_$(DEBUG_SUFFIX): CXXFLAGS := $(DEBUG_FLAGS) $(CXXFLAGS)
$(TARGET)_$(DEBUG_SUFFIX): $(DEBUG_OBJECTS)
	$(call build_elf, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_bin, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_hex, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_asm, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_dmp, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_sym, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_print_size, $(DEBUG_OBJECTS),$(DEBUG_DIR))
	$(call generate_hex_dump, $(DEBUG_DIR))





# Build release objects
%.o: %.c
	$(call build_object)


%.o: %.cpp
	$(call build_object)


%.o: %.S
	$(call build_object)



# Build debug objects
%_$(DEBUG_SUFFIX).o: %.c
	$(call build_object)


%_$(DEBUG_SUFFIX).o: %.cpp
	$(call build_object)


%_$(DEBUG_SUFFIX).o: %.S
	$(call build_object)





ifneq "$(MAKECMDGOALS)" "clean"

-include $(DEPENDENC)
-include $(DEBUG_DEPENDENC)

endif





.PHONY: clean
clean:
	-@$(RM_RECURSIVE) $(OBJECTS)
	-@$(RM_RECURSIVE) $(DEBUG_OBJECTS)
	-@$(RM_RECURSIVE) $(DEPENDENC)
	-@$(RM_RECURSIVE) $(DEBUG_DEPENDENC)





.PHONY: distclean
distclean: clean





ifeq "$(USE_CXX)" "yes"

define build_object
    @$(ECHO) [CXX build]: $@
    @$(CXX) -MMD -c $< -o $@ $(CXXFLAGS)
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define build_elf
    @$(ECHO) [CXX build]: $@
    @$(MKDIR) $2
    $(CXX) $(CXXFLAGS) $(LDFLAGS) -Wl,-Map=$2/$@.map $1 -o $2/$@.elf
endef

else

define build_object
    @$(ECHO) [CC build]: $@
    @$(CC) $(CCFLAGS) -MMD -c $< -o $@
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define build_elf
    @$(ECHO) [CC build]: $@
    @$(MKDIR) $2
    @$(CC) $(CCFLAGS) $(LDFLAGS) -Wl,-Map=$2/$@.map $1 -o $2/$@.elf
endef

endif



# $1 - list objects release and debug
# $2 - subdir release and debug
define generate_dump
	@$(OBJDUMP) $2/$@.elf -h > $2/$@.dmp
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define generate_bin
	@$(OBJCOPY) $2/$@.elf $2/$@.bin -O binary
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define generate_hex
	@$(OBJCOPY) $2/$@.elf $2/$@.hex -O ihex
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define generate_asm
	@$(OBJDUMP) -DS -C -m$(OBJDUMP_M) \
	--section=.text \
	--section=.data \
	--section=.bss \
	$2/$@.elf > $2/$@.asm
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define generate_sym
	@$(NM) -C -l -n -S $2/$@.elf > $2/$@.sym
endef



# $1 - list objects release and debug
# $2 - subdir release and debug
define generate_print_size
	@$(ECHO)
    @$(SIZE) -A $2/$@.elf
endef



# $1 - subdir release and debug
define generate_hex_dump
    @od -t x4 -v -An -w4 $1/$@.bin > $1/$@.txt
endef





.PHONY: help
help:
	@$(ECHO) "make [command]"
	@$(ECHO) "command is:"
	@$(ECHO) "   all       -  build release and debug mode"
	@$(ECHO) "   debug     -  build in debug mode (#define DEBUG 1)"
	@$(ECHO) "   release   -  build in release mode (strip)"
	@$(ECHO) "   clean     -  remove all generated files"
	@$(ECHO) "   help      -  this help"
