#
# Copyright 2019 AbbeyCatUK
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

#.SILENT:

# common
FIND     			= find
OPTIMIZE 			= 0

# 32-bit toolchain
GCC_PATH_32			= /gcc-arm-8.3-2019.03-x86_64-arm-eabi
GCC_32      			= $(GCC_PATH_32)/bin/arm-eabi-gcc
AS_32       			= $(GCC_PATH_32)/arm-eabi/bin/as
AR_32       			= $(GCC_PATH_32)/arm-eabi/bin/ar
LD_32       			= $(GCC_PATH_32)/arm-eabi/bin/ld
OBJDUMP_32  			= $(GCC_PATH_32)/arm-eabi/bin/objdump
OBJCOPY_32  			= $(GCC_PATH_32)/arm-eabi/bin/objcopy

# 64-bit toolchain
GCC_PATH_64			= /gcc-arm-8.3-2019.03-x86_64-aarch64-elf
GCC_64      			= $(GCC_PATH_64)/bin/aarch64-elf-gcc
AS_64       			= $(GCC_PATH_64)/aarch64-elf/bin/as
AR_64       			= $(GCC_PATH_64)/aarch64-elf/bin/ar
LD_64       			= $(GCC_PATH_64)/aarch64-elf/bin/ld
OBJDUMP_64  			= $(GCC_PATH_64)/aarch64-elf/bin/objdump
OBJCOPY_64  			= $(GCC_PATH_64)/aarch64-elf/bin/objcopy

# 32/64 bit flags for Assembler and Compiler
ARCH_64	 			= -march=armv8-a -mtune=cortex-a53
FLAGS_C_64  			= $(ARCH_64) -nostdlib -nostdinc -std=gnu99 -fsigned-char -Wno-psabi -O$(OPTIMIZE) -fno-builtin -nostartfiles -ffreestanding -D ISA_TYPE=$(ISA_TYPE)
FLAGS_A_64			= $(ARCH_64) -O$(OPTIMIZE) -D ISA_TYPE=$(ISA_TYPE)

ARCH_32	 			= -march=armv8-a -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
FLAGS_C_32  			= $(ARCH_32) -nostdlib -nostdinc -std=gnu99 -fsigned-char -Wno-psabi -O$(OPTIMIZE) -fno-builtin -nostartfiles -ffreestanding -mapcs-frame -mno-thumb-interwork -D ISA_TYPE=$(ISA_TYPE)
FLAGS_A_32			= $(ARCH_32) -O$(OPTIMIZE) -D ISA_TYPE=$(ISA_TYPE)

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

# 32-bit

32-bit: clean basic

32-bit: ISA_TYPE		= 32

32-bit: GCC			= $(GCC_32)
32-bit: AS			= $(AS_32)
32-bit: AR			= $(AR_32)
32-bit: LD			= $(LD_32)
32-bit: OBJDUMP			= $(OBJDUMP_32)
32-bit: OBJCOPY			= $(OBJCOPY_32)

32-bit: FLAGS_C			= $(FLAGS_C_32)
32-bit: FLAGS_A			= $(FLAGS_A_32)

32-bit: INCLUDE_DIR		= c/32-bit/include

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

# 64-bit

64-bit: clean basic

64-bit: ISA_TYPE		= 64

64-bit: GCC			= $(GCC_64)
64-bit: AS			= $(AS_64)
64-bit: AR			= $(AR_64)
64-bit: LD			= $(LD_64)
64-bit: OBJDUMP			= $(OBJDUMP_64)
64-bit: OBJCOPY			= $(OBJCOPY_64)

64-bit: FLAGS_C			= $(FLAGS_C_64)
64-bit: FLAGS_A			= $(FLAGS_A_64)

64-bit: INCLUDE_DIR		= c/64-bit/include/

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

# clean

clean:
	$(FIND) . -type f -name "*.o"    -delete
	$(FIND) . -type f -name "*.hex"  -delete
	$(FIND) . -type f -name "*.bin"  -delete
	$(FIND) . -type f -name "*.elf"  -delete
	$(FIND) . -type f -name "*.list" -delete

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

# basic

BASIC_HOME			= .
BASIC_INCLUDES		    	= -I $(BASIC_HOME)/c/common -I $(BASIC_HOME)/../stdlib/c/common/include

BASIC_FILES_C_MAIN     		= $(patsubst %.c,%.o,$(shell find $(BASIC_HOME)/c/common/basic/code/main      -type f -name '*.c'))
BASIC_FILES_C_CORE     		= $(patsubst %.c,%.o,$(shell find $(BASIC_HOME)/c/common/basic/code/core      -type f -name '*.c'))
BASIC_FILES_C_DIRECTIVE     	= $(patsubst %.c,%.o,$(shell find $(BASIC_HOME)/c/common/basic/code/directive -type f -name '*.c'))
BASIC_FILES_C_FUNCTION		= $(patsubst %.c,%.o,$(shell find $(BASIC_HOME)/c/common/basic/code/function  -type f -name '*.c'))
BASIC_FILES_C_LANGUAGE		= $(patsubst %.c,%.o,$(shell find $(BASIC_HOME)/c/common/basic/code/language  -type f -name '*.c'))
BASIC_FILES_C_OS		= $(patsubst %.c,%.o,$(shell find $(BASIC_HOME)/c/common/os/code              -type f -name '*.c'))

BASIC_FLAGS_C	         	= $(FLAGS_C) $(BASIC_INCLUDES)
BASIC_FILES_C     		= $(BASIC_FILES_C_MAIN) $(BASIC_FILES_C_CORE) $(BASIC_FILES_C_DIRECTIVE) $(BASIC_FILES_C_FUNCTION) $(BASIC_FILES_C_LANGUAGE) $(BASIC_FILES_C_OS) 

basic: basic_c
	$(LD)  	linker.ld -o basic.elf $(BASIC_FILES_C) ../stdlib/stdlib-$(ISA_TYPE).a

	$(OBJCOPY) -I elf32-little -O binary --strip-debug --strip-unneeded --verbose basic.elf basic.bin 
	$(OBJCOPY) basic.elf -O ihex basic.hex
	$(OBJDUMP) -D basic.elf > basic.list

basic_c: $(BASIC_FILES_C)

$(BASIC_HOME)/%.o : $(BASIC_HOME)/%.c
	$(GCC) -c $(BASIC_FLAGS_C) $< -o $@
 
