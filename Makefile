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

GCC_PATH		= ~/Downloads/Software/ARM/32-bit/gcc-arm-8.2-2019.01-x86_64-arm-eabi/
GCC      		= $(GCC_PATH)/bin/arm-eabi-gcc
AS       		= $(GCC_PATH)/arm-eabi/bin/as
AR       		= $(GCC_PATH)/arm-eabi/bin/ar
LD       		= $(GCC_PATH)/arm-eabi/bin/ld
OBJDUMP  		= $(GCC_PATH)/arm-eabi/bin/objdump
OBJCOPY  		= $(GCC_PATH)/arm-eabi/bin/objcopy


FIND     		= find
OPTIMIZE 		= -O0
ARCH	 		= -march=armv8-a -mtune=cortex-a53 -mfloat-abi=hard -mfpu=neon-fp-armv8
FLAGS_C  		= $(ARCH) -std=gnu99 -fsigned-char -undef -Wall -Wno-psabi $(OPTIMIZE) -fno-builtin -nostdinc -nostartfiles -ffreestanding -mapcs-frame -mno-thumb-interwork
FLAGS_A			= $(ARCH) $(OPTIMIZE)

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

# all

all: clean basic

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

# clean

clean:
	$(FIND) . -type f -name "*.o"    -delete
	$(FIND) . -type f -name "*.hex"  -delete
	$(FIND) . -type f -name "*.bin"  -delete
	$(FIND) . -type f -name "*.elf"  -delete
	$(FIND) . -type f -name "*.list" -delete

# -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

BASIC_HOME 			= .

BASIC_INCLUDES		= -I .
BASIC_FLAGS_C	 	= $(FLAGS_C) $(BASIC_INCLUDES)

BASIC_CORE_FILES_C   	= $(patsubst %.c,%.o,$(wildcard $(BASIC_HOME)/basic/code/core/*.c))
BASIC_DIRECTIVE_FILES_C = $(patsubst %.c,%.o,$(wildcard $(BASIC_HOME)/basic/code/directive/*.c))
BASIC_FUNCTION_FILES_C  = $(patsubst %.c,%.o,$(wildcard $(BASIC_HOME)/basic/code/function/*.c))
BASIC_LANGUAGE_FILES_C  = $(patsubst %.c,%.o,$(wildcard $(BASIC_HOME)/basic/code/language/*.c))
OS_KBD_FILES_C   		= $(patsubst %.c,%.o,$(wildcard $(BASIC_HOME)/os/code/keyboard/*.c))
OS_VID_FILES_C   		= $(patsubst %.c,%.o,$(wildcard $(BASIC_HOME)/os/code/video/*.c))

basic: basic_core basic_directive basic_function basic_language os_keyboard os_video

	$(LD) linker.ld -o basic.elf \
	${BASIC_HOME}/basic/code/core/*.o ${BASIC_HOME}/basic/code/directive/*.o ${BASIC_HOME}/basic/code/function/*.o ${BASIC_HOME}/basic/code/language/*.o \
	${BASIC_HOME}/os/code/keyboard/*.o ${BASIC_HOME}/os/code/video/*.o \
	ext/stdlib/stdlib.a

	$(OBJCOPY) -I elf32-little -O binary --strip-debug --strip-unneeded --verbose basic.elf basic.bin 
	$(OBJCOPY) basic.elf -O ihex basic.hex
	$(OBJDUMP) -D basic.elf > basic.list

basic_core:      $(BASIC_CORE_FILES_C)
basic_directive: $(BASIC_DIRECTIVE_FILES_C)
basic_function:  $(BASIC_FUNCTION_FILES_C)
basic_language:  $(BASIC_LANGUAGE_FILES_C)
os_keyboard: 	 $(OS_KBD_FILES_C)
os_video: 		 $(OS_VID_FILES_C)

$(BASIC_HOME)/%.o : $(BASIC_HOME)/%.c
	$(GCC) -c $(BASIC_FLAGS_C) $< -o $@  
