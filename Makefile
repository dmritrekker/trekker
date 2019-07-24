# Cross compilation for Windows is tested and done using mingw on a Debian 9 system.
# It requires 3 packages: (1) g++-mingw-w64 (2) libz-mingw-w64 (3) libz-mingw-w64-dev
# Last 2 are for zlib support used for decompressing .nii.gz files
BUILD_FOR_WINDOWS     = NO
DEBUG_MODE            = NO
ENABLE_ZLIB 	      = NO
ENABLE_MULTITHREADING = NO
BUILD_SHARED 		  = NO

SHELL:=/bin/bash

ifeq (${BUILD_FOR_WINDOWS},YES)
# G++                   = x86_64-w64-mingw32-g++ -D BUILD_FOR_WINDOWS
G++                   = g++ -D BUILD_FOR_WINDOWS
MSLIBTOOL             = /C/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x64/lib.exe
DLLTOOL 			  = dlltool
MANUALCMD   		  = "start /max file:///$(abspath doc/build/html/index.html)"
# arCommand             = x86_64-w64-mingw32-ar
arCommand             = ar
libtrekker 			  = libtrekker
trekkerCMD 		  	  = trekker.exe
else
G++                   = g++
MANUALCMD   		  = "man $(abspath doc/build/man/trekker)"
arCommand             = ar
libtrekker 	     	  = libtrekker.a
trekkerCMD 		  	  = trekker
endif

trekker_CXXFLAGS      = -DSHOWMANUAL='${MANUALCMD}'

ifeq (${DEBUG_MODE},YES)
ENABLE_MULTITHREADING = NO
CXX             = ${G++} -static -static-libgcc -static-libstdc++ -std=c++11 -fpic -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -O0 -ggdb3 -fno-omit-frame-pointer
# CXX             = ${G++} -std=c++11 -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -O0 -ggdb3 -fno-omit-frame-pointer
else
CXX             = ${G++} -static -static-libgcc -static-libstdc++ -std=c++11 -fpic -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -O3
# CXX             = ${G++} -std=c++11 -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -O3
endif

# Configure build directory
BUILD_DIR       = ./build
BUILD_BIN       = ${BUILD_DIR}/bin
BUILD_LIB       = ${BUILD_DIR}/lib
BUILD_INC       = ${BUILD_DIR}/include
BUILD_OBJ       = ${BUILD_DIR}/lib/obj


# Configure nifti compilation
NIFTI_SRCPATH 	= ./src/nifticlib-2.0.0/niftilib
NIFTI_SRCPATH  += ./src/nifticlib-2.0.0/znzlib
NIFTI_SRC       = $(shell echo $(addsuffix /*.c, ${NIFTI_SRCPATH}))

NIFTI_OBJ       = $(addprefix ${BUILD_OBJ}/, $(addsuffix .o, $(notdir $(basename ${NIFTI_SRC}))))
NIFTI_DEP       = $(addprefix ${BUILD_OBJ}/, $(addsuffix .d, $(notdir $(basename ${NIFTI_SRC}))))

NIFTI_INC       = -I./src/nifticlib-2.0.0/niftilib -I./src/nifticlib-2.0.0/znzlib  
NIFTI_LIB       = -lm

NIFTI_CXXFLAGS  = -ansi -w
ifeq (${ENABLE_ZLIB},YES)
NIFTI_CXXFLAGS += -D HAVE_ZLIB
NIFTI_LIB      += -lz
endif


# Configure trekker compilation
trekker_SRCPATH = ./src
trekker_SRCPATH+= ./src/base
trekker_SRCPATH+= ./src/config
trekker_SRCPATH+= ./src/math
trekker_SRCPATH+= ./src/image
trekker_SRCPATH+= ./src/tracker
trekker_SRCPATH+= ./src/tracker/algorithms/ptt
trekker_SRCPATH+= ./src/tracker/algorithms/local_probabilistic
trekker_SRC     = $(shell echo $(addsuffix /*.cpp, ${trekker_SRCPATH}))

trekker_OBJ     = $(addprefix ${BUILD_OBJ}/, $(addsuffix .o, $(notdir $(basename ${trekker_SRC}))))
trekker_DEP     = $(addprefix ${BUILD_OBJ}/, $(addsuffix .d, $(notdir $(basename ${trekker_SRC}))))

trekker_INC 		= ${NIFTI_INC}
trekker_LIB     	= ${NIFTI_LIB}
trekker         	= ${BUILD_BIN}/${trekkerCMD}

ifeq (${ENABLE_MULTITHREADING},YES)
trekker_LIB      	+= -lpthread
trekker_CXXFLAGS 	+= -D ENABLE_MULTITHREADING
endif

# Configure for building the shared library
libtrekker_OBJ     	 = $(addprefix ${BUILD_OBJ}/, $(addsuffix .o, $(notdir $(basename ${NIFTI_SRC}))))
libtrekker_OBJ     	+= $(addprefix ${BUILD_OBJ}/, $(addsuffix .o, $(notdir $(basename ${trekker_SRC}))))
libtrekker_DEP     	 = $(addprefix ${BUILD_OBJ}/, $(addsuffix .d, $(notdir $(basename ${NIFTI_SRC}))))
libtrekker_DEP     	+= $(addprefix ${BUILD_OBJ}/, $(addsuffix .d, $(notdir $(basename ${trekker_SRC}))))

vpath %.c ${NIFTI_SRCPATH}
vpath %.cpp ${trekker_SRCPATH}


.PHONY: all
all: ${BUILD_DIR}
	@${MAKE} ${trekker}	
	@if [ "${BUILD_SHARED}" = "YES" ]; then ${MAKE} ${libtrekker}; fi
	
${BUILD_DIR}:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${BUILD_LIB}
	@mkdir -p ${BUILD_BIN}
	@mkdir -p ${BUILD_OBJ}	
	@if [ "${BUILD_SHARED}" = "YES" ]; then mkdir -p ${BUILD_INC}; fi
	@echo "./build directory created"


# Targets for trekker
-include ${trekker_DEP} ${NIFTI_DEP}

${BUILD_OBJ}/%.o: %.c
	@echo "Building $<"
	@$(CXX) -MMD ${NIFTI_CXXFLAGS} ${NIFTI_INC} -c $< -o $@

${BUILD_OBJ}/%.o: %.cpp
	@echo "Building $<"
	@$(CXX) -MMD ${trekker_CXXFLAGS} ${trekker_INC} -c $< -o $@

${trekker}: ${NIFTI_OBJ} ${trekkerc} ${trekker_OBJ}
	@$(CXX) ${trekker_CXXFLAGS} $^ -o $@ ${trekker_LIB}
	@if [ "${ENABLE_ZLIB}"             = "YES" ]; then echo "Enabled zlib support";             else echo "Disabled zlib support";             fi
	@if [ "${ENABLE_MULTITHREADING}"   = "YES" ]; then echo "Enabled multithreading support";   else echo "Disabled multithreading support";   fi
	@echo "$@ successfully built"
	
# Targets for shared library
-include ${libtrekker_DEP}

# @if [ "${BUILD_FOR_WINDOWS}" = "YES" ]; then \
# ${G++} -shared -o ${BUILD_LIB}/${libtrekker}.dll $^ ${trekker_LIB} -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--output-def,${BUILD_LIB}/${libtrekker}.def,--out-implib,${BUILD_LIB}/${libtrekker}.a; cd ${BUILD_LIB}; ${MSLIBTOOL} /def:${libtrekker}.def /out:${libtrekker}.lib; cd ../.. ; else \
# ${arCommand} -rcs ${BUILD_LIB}/${libtrekker} $^; fi

# ${arCommand} -rcs ${BUILD_LIB}/${libtrekker} $^

# ${G++} -o ${BUILD_LIB}/${libtrekker}.dll $^ -shared -s -Wl,--subsystem,windows,--output-def,${BUILD_LIB}/${libtrekker}.def; \
# ${G++} -o ${BUILD_LIB}/${libtrekker}.dll $^ -shared -s -Wl,--subsystem,windows,--kill-at; \
# ${DLLTOOL} --kill-at -d ${BUILD_LIB}/${libtrekker}.def -D ${BUILD_LIB}/${libtrekker}.dll -l ${BUILD_LIB}/${libtrekker}.a; \


${libtrekker}: ${libtrekker_OBJ}
	@echo "Building shared library"
	@if [ "${BUILD_FOR_WINDOWS}" = "YES" ]; then \
	${G++} -o ${BUILD_LIB}/${libtrekker}.dll $^ ${trekker_LIB} -shared -s -Wl,--subsystem,windows,--output-def,${BUILD_LIB}/${libtrekker}.def; \
	${G++} -o ${BUILD_LIB}/${libtrekker}.dll $^ ${trekker_LIB} -shared -s -Wl,--subsystem,windows,--kill-at; \
	${DLLTOOL} --kill-at -d ${BUILD_LIB}/${libtrekker}.def -D ${BUILD_LIB}/${libtrekker}.dll -l ${BUILD_LIB}/${libtrekker}.a; \
	cd ${BUILD_LIB}; ${MSLIBTOOL} /def:${libtrekker}.def /out:${libtrekker}.lib; cd ../.. ; \
	else \
	${arCommand} -rcs ${BUILD_LIB}/${libtrekker} $^; fi
	@cp ./src/*.h ${BUILD_INC}/
	@echo "$@ successfully built"

.PHONY: clean
clean:
	@rm -rf ./build
	@echo "./build directory removed"
	