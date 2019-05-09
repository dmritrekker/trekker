G++                   = g++
DEBUG_MODE            = NO
ENABLE_ZLIB 	      = YES
ENABLE_MULTITHREADING = YES
STATIC_COMPILATION    = YES

# The following requires 3 packages g++-mingw-w64, libz-mingw-w64 and libz-mingw-w64-dev. Last 2 are for zlib support.
BUILD_FOR_WINDOWS     = NO

SHELL:=/bin/bash

# STAND ALONE WINDOWS BINARY
# A windows x64 executable is compiled from a Linux machine using mingw.
# The following setup works well using Debian Stretch (version 9).
# This compilation requires 3 packages g++-mingw-w64, libz-mingw-w64 and libz-mingw-w64-dev. Last 2 are for zlib support.
ifeq (${BUILD_FOR_WINDOWS},YES)
G++                   = x86_64-w64-mingw32-g++ -std=c++11
trekker_CXXFLAGS      = -D BUILD_FOR_WINDOWS
trekker_LIB_WIN       = -static -static-libgcc -static-libstdc++
MANUALCMD   		  = "start /max file:///$(abspath doc/build/html/index.html)"
else
MANUALCMD   		  = "man $(abspath doc/build/man/trekker)"
endif

trekker_CXXFLAGS     += -DSHOWMANUAL='${MANUALCMD}'

ifeq (${DEBUG_MODE},YES)
ENABLE_MULTITHREADING = NO
CXX             = ${G++} -std=c++11 -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -O0 -ggdb3 -fno-omit-frame-pointer
else
CXX             = ${G++} -std=c++11 -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -O3
endif

BUILD_DIR       = ./build
BUILD_BIN       = ${BUILD_DIR}/bin
BUILD_LIB       = ${BUILD_DIR}/lib
BUILD_INC       = ${BUILD_DIR}/include
BUILD_OBJ       = ${BUILD_DIR}/lib/obj

NIFTI_CXXFLAGS  = -ansi -w
NIFTI_INC       = -I./src/nifticlib-2.0.0/niftilib -I./src/nifticlib-2.0.0/znzlib  
NIFTI_LIB       = -lm
ifeq (${ENABLE_ZLIB},YES)
NIFTI_CXXFLAGS += -D HAVE_ZLIB
NIFTI_LIB      += -lz
endif
NIFTI           = ${BUILD_LIB}/libnifti.a

ifeq (${BUILD_FOR_WINDOWS},YES)
NIFTI_SHARED    = ${BUILD_LIB}/libnifti.dll
else
NIFTI_SHARED    = ${BUILD_LIB}/libnifti.so
endif

trekker_SRCPATH = ./src
trekker_SRCPATH+= ./src/base
trekker_SRCPATH+= ./src/config
trekker_SRCPATH+= ./src/math
trekker_SRCPATH+= ./src/image
trekker_SRCPATH+= ./src/tracker
trekker_SRCPATH+= ./src/tracker/algorithms/ptt
trekker_SRCPATH+= ./src/tracker/algorithms/local_probabilistic
trekker_SRC     = $(shell echo $(addsuffix /*.cpp, ${trekker_SRCPATH}))

trekker_OBJ_DIR = ${BUILD_OBJ}
trekker_OBJ     = $(addprefix ${trekker_OBJ_DIR}/, $(addsuffix .o, $(notdir $(basename ${trekker_SRC}))))
trekker_DEP_DIR = ${BUILD_OBJ}
trekker_DEP     = $(addprefix ${trekker_DEP_DIR}/, $(addsuffix .d, $(notdir $(basename ${trekker_SRC}))))

trekker_INC 		= ${NIFTI_INC}
trekker_LIB     	= ${NIFTI} ${NIFTI_LIB}
trekker_LIB_SHARED  = ${NIFTI_SHARED} ${NIFTI_LIB}
ifeq (${STATIC_COMPILATION},YES)
trekker_LIB    += -static -static-libgcc -static-libstdc++
endif
trekker         = ${BUILD_BIN}/trekker

ifeq (${ENABLE_MULTITHREADING},YES)
trekker_LIB      	+= -lpthread
trekker_LIB_SHARED 	+= -lpthread
trekker_CXXFLAGS 	+= -D ENABLE_MULTITHREADING
endif

libtrekker_OBJ_DIR 	 = ${BUILD_OBJ}
libtrekker_OBJ     	 = $(addprefix ${libtrekker_OBJ_DIR}/, $(addsuffix .o, $(notdir $(basename ${trekker_SRC}))))
libtrekker_DEP_DIR 	 = ${BUILD_OBJ}
libtrekker_DEP     	 = $(addprefix ${libtrekker_DEP_DIR}/, $(addsuffix .d, $(notdir $(basename ${trekker_SRC}))))

vpath %.cpp ${trekker_SRCPATH}

.PHONY: all
all: ${BUILD_DIR} ${NIFTI}
	@${MAKE} ${trekker}	
	@if [ "${BUILD_SHARED}"       = "YES" ]; \
	then \
	${MAKE} ${NIFTI_SHARED}; \
	fi
	
${BUILD_DIR}:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${BUILD_LIB}
	@mkdir -p ${BUILD_BIN}
	@mkdir -p ${BUILD_OBJ}
	@mkdir -p ${libtrekker_OBJ_DIR}
	@echo "./build directory created"

# Build trekker
${NIFTI}: ${BUILD_DIR}
	@$(CXX) $(NIFTI_CXXFLAGS) ${NIFTI_INC} -c ./src/nifticlib-2.0.0/znzlib/znzlib.c -o ${BUILD_LIB}/znzlib.o ${NIFTI_LIB}
	@$(CXX) $(NIFTI_CXXFLAGS) ${NIFTI_INC} -c ./src/nifticlib-2.0.0/niftilib/nifti1_io.c -o ${BUILD_LIB}/nifti1_io.o ${NIFTI_LIB}
	@ar -r ${NIFTI} ${BUILD_LIB}/znzlib.o ${BUILD_LIB}/nifti1_io.o
	@rm -f ${BUILD_LIB}/znzlib.o ${BUILD_LIB}/nifti1_io.o

-include ${trekker_DEP}

${trekker_OBJ_DIR}/%.o: %.cpp
	@echo "Building $<"
	@$(CXX) -MMD ${trekker_CXXFLAGS} ${trekker_INC} -c $< -o $@

${trekker}: ${trekker_OBJ}
	@if [ "${BUILD_FOR_WINDOWS}"       = "YES" ]; \
	then $(CXX) ${trekker_CXXFLAGS} ${trekker_INC} $^ -o $@.exe ${trekker_LIB} ${trekker_LIB_WIN}; \
	else $(CXX) ${trekker_CXXFLAGS} ${trekker_INC} $^ -o $@ ${trekker_LIB}; \
	fi
	@if [ "${ENABLE_ZLIB}"             = "YES" ]; then echo "Enabled zlib support";             else echo "Disabled zlib support";             fi
	@if [ "${ENABLE_MULTITHREADING}"   = "YES" ]; then echo "Enabled multithreading support";   else echo "Disabled multithreading support";   fi
	@if [ "${BUILD_FOR_WINDOWS}"       = "YES" ]; \
	then echo "$@.exe successfully built"; \
	else echo "$@ successfully built"; \
	fi

.PHONY: clean
clean:
	@rm -rf ./build
	@echo "./build directory removed"	
