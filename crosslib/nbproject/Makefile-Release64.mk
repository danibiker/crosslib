#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW64-Windows
CND_DLIB_EXT=dll
CND_CONF=Release64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/ca5adee5/AudioCD_Helpers.o \
	${OBJECTDIR}/_ext/ca5adee5/CAudioCD.o \
	${OBJECTDIR}/_ext/1e83243d/audioplayer.o \
	${OBJECTDIR}/_ext/1e83243d/biquad.o \
	${OBJECTDIR}/_ext/23c3a6e2/Fft.o \
	${OBJECTDIR}/_ext/64c357a0/freedb.o \
	${OBJECTDIR}/_ext/f109d26e/htmlparser.o \
	${OBJECTDIR}/_ext/f109d26e/httputil.o \
	${OBJECTDIR}/_ext/3400367a/uiimgdownloader.o \
	${OBJECTDIR}/_ext/3400367a/uiimgencoder.o \
	${OBJECTDIR}/_ext/51d5feb5/MusixMatch.o \
	${OBJECTDIR}/_ext/51d5feb5/darklyrics.o \
	${OBJECTDIR}/_ext/51d5feb5/lyricsbase.o \
	${OBJECTDIR}/_ext/51d5feb5/lyricswikia.o \
	${OBJECTDIR}/_ext/391a7aef/allmyvideos.o \
	${OBJECTDIR}/_ext/391a7aef/dropbox.o \
	${OBJECTDIR}/_ext/391a7aef/googledrive.o \
	${OBJECTDIR}/_ext/391a7aef/ioauth2.o \
	${OBJECTDIR}/_ext/391a7aef/onedrive.o \
	${OBJECTDIR}/_ext/391a7aef/streamcloud.o \
	${OBJECTDIR}/_ext/391a7aef/vk.o \
	${OBJECTDIR}/_ext/64cbc303/ConstantHttp.o \
	${OBJECTDIR}/_ext/64cbc303/Progress.o \
	${OBJECTDIR}/_ext/a8c489d9/filecipher.o \
	${OBJECTDIR}/_ext/a8c489d9/rijndael.o \
	${OBJECTDIR}/_ext/a8c489d9/sha256.o \
	${OBJECTDIR}/_ext/20f55786/Colorutil.o \
	${OBJECTDIR}/_ext/20f55786/Constant.o \
	${OBJECTDIR}/_ext/20f55786/Dirutil.o \
	${OBJECTDIR}/_ext/20f55786/Excepcion.o \
	${OBJECTDIR}/_ext/20f55786/Fileio.o \
	${OBJECTDIR}/_ext/20f55786/ImagenGestor.o \
	${OBJECTDIR}/_ext/20f55786/Launcher.o \
	${OBJECTDIR}/_ext/20f55786/ListaSimple.o \
	${OBJECTDIR}/_ext/20f55786/Menuobject.o \
	${OBJECTDIR}/_ext/20f55786/SHA1.o \
	${OBJECTDIR}/_ext/20f55786/Traza.o \
	${OBJECTDIR}/_ext/20f55786/UITreeListBox.o \
	${OBJECTDIR}/_ext/8d36c5a9/Database.o \
	${OBJECTDIR}/_ext/19a342fa/listgroupcol.o \
	${OBJECTDIR}/_ext/20f55786/cursor.o \
	${OBJECTDIR}/_ext/20f55786/filelaunch.o \
	${OBJECTDIR}/_ext/20f55786/joymapper.o \
	${OBJECTDIR}/_ext/20f55786/object.o \
	${OBJECTDIR}/_ext/20f55786/registry.o \
	${OBJECTDIR}/_ext/d3554a4c/SOUtils.o \
	${OBJECTDIR}/_ext/20f55786/uiart.o \
	${OBJECTDIR}/_ext/20f55786/uibutton.o \
	${OBJECTDIR}/_ext/20f55786/uicheck.o \
	${OBJECTDIR}/_ext/20f55786/uicombobox.o \
	${OBJECTDIR}/_ext/20f55786/uiinput.o \
	${OBJECTDIR}/_ext/20f55786/uilabel.o \
	${OBJECTDIR}/_ext/20f55786/uilist.o \
	${OBJECTDIR}/_ext/20f55786/uilistcommon.o \
	${OBJECTDIR}/_ext/20f55786/uilistgroup.o \
	${OBJECTDIR}/_ext/20f55786/uipanel.o \
	${OBJECTDIR}/_ext/20f55786/uipanelborder.o \
	${OBJECTDIR}/_ext/20f55786/uipicture.o \
	${OBJECTDIR}/_ext/20f55786/uipopupmenu.o \
	${OBJECTDIR}/_ext/20f55786/uiprogressbar.o \
	${OBJECTDIR}/_ext/20f55786/uislider.o \
	${OBJECTDIR}/_ext/20f55786/uispectrum.o \
	${OBJECTDIR}/_ext/20f55786/uitextelementsarea.o \
	${OBJECTDIR}/_ext/1ab3761f/unziptool.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fpermissive
CXXFLAGS=-fpermissive

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrosslib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrosslib.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrosslib.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrosslib.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrosslib.a

${OBJECTDIR}/_ext/ca5adee5/AudioCD_Helpers.o: ../src/audioplayer/audiocd/AudioCD_Helpers.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/ca5adee5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/ca5adee5/AudioCD_Helpers.o ../src/audioplayer/audiocd/AudioCD_Helpers.cpp

${OBJECTDIR}/_ext/ca5adee5/CAudioCD.o: ../src/audioplayer/audiocd/CAudioCD.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/ca5adee5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/ca5adee5/CAudioCD.o ../src/audioplayer/audiocd/CAudioCD.cpp

${OBJECTDIR}/_ext/1e83243d/audioplayer.o: ../src/audioplayer/audioplayer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/1e83243d
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1e83243d/audioplayer.o ../src/audioplayer/audioplayer.cpp

${OBJECTDIR}/_ext/1e83243d/biquad.o: ../src/audioplayer/biquad.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/1e83243d
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1e83243d/biquad.o ../src/audioplayer/biquad.cpp

${OBJECTDIR}/_ext/23c3a6e2/Fft.o: ../src/audioplayer/fft/Fft.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/23c3a6e2
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/23c3a6e2/Fft.o ../src/audioplayer/fft/Fft.cpp

${OBJECTDIR}/_ext/64c357a0/freedb.o: ../src/httpcurl/cddb/freedb.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/64c357a0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/64c357a0/freedb.o ../src/httpcurl/cddb/freedb.cpp

${OBJECTDIR}/_ext/f109d26e/htmlparser.o: ../src/httpcurl/htmlparser.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f109d26e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f109d26e/htmlparser.o ../src/httpcurl/htmlparser.cpp

${OBJECTDIR}/_ext/f109d26e/httputil.o: ../src/httpcurl/httputil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f109d26e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f109d26e/httputil.o ../src/httpcurl/httputil.cpp

${OBJECTDIR}/_ext/3400367a/uiimgdownloader.o: ../src/httpcurl/image/uiimgdownloader.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/3400367a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3400367a/uiimgdownloader.o ../src/httpcurl/image/uiimgdownloader.cpp

${OBJECTDIR}/_ext/3400367a/uiimgencoder.o: ../src/httpcurl/image/uiimgencoder.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/3400367a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3400367a/uiimgencoder.o ../src/httpcurl/image/uiimgencoder.cpp

${OBJECTDIR}/_ext/51d5feb5/MusixMatch.o: ../src/httpcurl/lyrics/MusixMatch.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/MusixMatch.o ../src/httpcurl/lyrics/MusixMatch.cpp

${OBJECTDIR}/_ext/51d5feb5/darklyrics.o: ../src/httpcurl/lyrics/darklyrics.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/darklyrics.o ../src/httpcurl/lyrics/darklyrics.cpp

${OBJECTDIR}/_ext/51d5feb5/lyricsbase.o: ../src/httpcurl/lyrics/lyricsbase.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/lyricsbase.o ../src/httpcurl/lyrics/lyricsbase.cpp

${OBJECTDIR}/_ext/51d5feb5/lyricswikia.o: ../src/httpcurl/lyrics/lyricswikia.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/lyricswikia.o ../src/httpcurl/lyrics/lyricswikia.cpp

${OBJECTDIR}/_ext/391a7aef/allmyvideos.o: ../src/httpcurl/servers/allmyvideos.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/allmyvideos.o ../src/httpcurl/servers/allmyvideos.cpp

${OBJECTDIR}/_ext/391a7aef/dropbox.o: ../src/httpcurl/servers/dropbox.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/dropbox.o ../src/httpcurl/servers/dropbox.cpp

${OBJECTDIR}/_ext/391a7aef/googledrive.o: ../src/httpcurl/servers/googledrive.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/googledrive.o ../src/httpcurl/servers/googledrive.cpp

${OBJECTDIR}/_ext/391a7aef/ioauth2.o: ../src/httpcurl/servers/ioauth2.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/ioauth2.o ../src/httpcurl/servers/ioauth2.cpp

${OBJECTDIR}/_ext/391a7aef/onedrive.o: ../src/httpcurl/servers/onedrive.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/onedrive.o ../src/httpcurl/servers/onedrive.cpp

${OBJECTDIR}/_ext/391a7aef/streamcloud.o: ../src/httpcurl/servers/streamcloud.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/streamcloud.o ../src/httpcurl/servers/streamcloud.cpp

${OBJECTDIR}/_ext/391a7aef/vk.o: ../src/httpcurl/servers/vk.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/vk.o ../src/httpcurl/servers/vk.cpp

${OBJECTDIR}/_ext/64cbc303/ConstantHttp.o: ../src/httpcurl/util/ConstantHttp.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/64cbc303
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/64cbc303/ConstantHttp.o ../src/httpcurl/util/ConstantHttp.cpp

${OBJECTDIR}/_ext/64cbc303/Progress.o: ../src/httpcurl/util/Progress.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/64cbc303
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/64cbc303/Progress.o ../src/httpcurl/util/Progress.cpp

${OBJECTDIR}/_ext/a8c489d9/filecipher.o: ../src/rijndael/filecipher.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a8c489d9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a8c489d9/filecipher.o ../src/rijndael/filecipher.cpp

${OBJECTDIR}/_ext/a8c489d9/rijndael.o: ../src/rijndael/rijndael.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a8c489d9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a8c489d9/rijndael.o ../src/rijndael/rijndael.cpp

${OBJECTDIR}/_ext/a8c489d9/sha256.o: ../src/rijndael/sha256.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a8c489d9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a8c489d9/sha256.o ../src/rijndael/sha256.cpp

${OBJECTDIR}/_ext/20f55786/Colorutil.o: ../src/uiobjects/Colorutil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Colorutil.o ../src/uiobjects/Colorutil.cpp

${OBJECTDIR}/_ext/20f55786/Constant.o: ../src/uiobjects/Constant.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Constant.o ../src/uiobjects/Constant.cpp

${OBJECTDIR}/_ext/20f55786/Dirutil.o: ../src/uiobjects/Dirutil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Dirutil.o ../src/uiobjects/Dirutil.cpp

${OBJECTDIR}/_ext/20f55786/Excepcion.o: ../src/uiobjects/Excepcion.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Excepcion.o ../src/uiobjects/Excepcion.cpp

${OBJECTDIR}/_ext/20f55786/Fileio.o: ../src/uiobjects/Fileio.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Fileio.o ../src/uiobjects/Fileio.cpp

${OBJECTDIR}/_ext/20f55786/ImagenGestor.o: ../src/uiobjects/ImagenGestor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/ImagenGestor.o ../src/uiobjects/ImagenGestor.cpp

${OBJECTDIR}/_ext/20f55786/Launcher.o: ../src/uiobjects/Launcher.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Launcher.o ../src/uiobjects/Launcher.cpp

${OBJECTDIR}/_ext/20f55786/ListaSimple.o: ../src/uiobjects/ListaSimple.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/ListaSimple.o ../src/uiobjects/ListaSimple.cpp

${OBJECTDIR}/_ext/20f55786/Menuobject.o: ../src/uiobjects/Menuobject.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Menuobject.o ../src/uiobjects/Menuobject.cpp

${OBJECTDIR}/_ext/20f55786/SHA1.o: ../src/uiobjects/SHA1.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/SHA1.o ../src/uiobjects/SHA1.cpp

${OBJECTDIR}/_ext/20f55786/Traza.o: ../src/uiobjects/Traza.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Traza.o ../src/uiobjects/Traza.cpp

${OBJECTDIR}/_ext/20f55786/UITreeListBox.o: ../src/uiobjects/UITreeListBox.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/UITreeListBox.o ../src/uiobjects/UITreeListBox.cpp

${OBJECTDIR}/_ext/8d36c5a9/Database.o: ../src/uiobjects/bbdd/Database.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/8d36c5a9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/8d36c5a9/Database.o ../src/uiobjects/bbdd/Database.cpp

${OBJECTDIR}/_ext/19a342fa/listgroupcol.o: ../src/uiobjects/beans/listgroupcol.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/19a342fa
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/19a342fa/listgroupcol.o ../src/uiobjects/beans/listgroupcol.cpp

${OBJECTDIR}/_ext/20f55786/cursor.o: ../src/uiobjects/cursor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/cursor.o ../src/uiobjects/cursor.cpp

${OBJECTDIR}/_ext/20f55786/filelaunch.o: ../src/uiobjects/filelaunch.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/filelaunch.o ../src/uiobjects/filelaunch.cpp

${OBJECTDIR}/_ext/20f55786/joymapper.o: ../src/uiobjects/joymapper.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/joymapper.o ../src/uiobjects/joymapper.cpp

${OBJECTDIR}/_ext/20f55786/object.o: ../src/uiobjects/object.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/object.o ../src/uiobjects/object.cpp

${OBJECTDIR}/_ext/20f55786/registry.o: ../src/uiobjects/registry.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/registry.o ../src/uiobjects/registry.cpp

${OBJECTDIR}/_ext/d3554a4c/SOUtils.o: ../src/uiobjects/soutils/SOUtils.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/d3554a4c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/d3554a4c/SOUtils.o ../src/uiobjects/soutils/SOUtils.cpp

${OBJECTDIR}/_ext/20f55786/uiart.o: ../src/uiobjects/uiart.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uiart.o ../src/uiobjects/uiart.cpp

${OBJECTDIR}/_ext/20f55786/uibutton.o: ../src/uiobjects/uibutton.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uibutton.o ../src/uiobjects/uibutton.cpp

${OBJECTDIR}/_ext/20f55786/uicheck.o: ../src/uiobjects/uicheck.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uicheck.o ../src/uiobjects/uicheck.cpp

${OBJECTDIR}/_ext/20f55786/uicombobox.o: ../src/uiobjects/uicombobox.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uicombobox.o ../src/uiobjects/uicombobox.cpp

${OBJECTDIR}/_ext/20f55786/uiinput.o: ../src/uiobjects/uiinput.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uiinput.o ../src/uiobjects/uiinput.cpp

${OBJECTDIR}/_ext/20f55786/uilabel.o: ../src/uiobjects/uilabel.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilabel.o ../src/uiobjects/uilabel.cpp

${OBJECTDIR}/_ext/20f55786/uilist.o: ../src/uiobjects/uilist.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilist.o ../src/uiobjects/uilist.cpp

${OBJECTDIR}/_ext/20f55786/uilistcommon.o: ../src/uiobjects/uilistcommon.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilistcommon.o ../src/uiobjects/uilistcommon.cpp

${OBJECTDIR}/_ext/20f55786/uilistgroup.o: ../src/uiobjects/uilistgroup.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilistgroup.o ../src/uiobjects/uilistgroup.cpp

${OBJECTDIR}/_ext/20f55786/uipanel.o: ../src/uiobjects/uipanel.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipanel.o ../src/uiobjects/uipanel.cpp

${OBJECTDIR}/_ext/20f55786/uipanelborder.o: ../src/uiobjects/uipanelborder.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipanelborder.o ../src/uiobjects/uipanelborder.cpp

${OBJECTDIR}/_ext/20f55786/uipicture.o: ../src/uiobjects/uipicture.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipicture.o ../src/uiobjects/uipicture.cpp

${OBJECTDIR}/_ext/20f55786/uipopupmenu.o: ../src/uiobjects/uipopupmenu.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipopupmenu.o ../src/uiobjects/uipopupmenu.cpp

${OBJECTDIR}/_ext/20f55786/uiprogressbar.o: ../src/uiobjects/uiprogressbar.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uiprogressbar.o ../src/uiobjects/uiprogressbar.cpp

${OBJECTDIR}/_ext/20f55786/uislider.o: ../src/uiobjects/uislider.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uislider.o ../src/uiobjects/uislider.cpp

${OBJECTDIR}/_ext/20f55786/uispectrum.o: ../src/uiobjects/uispectrum.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uispectrum.o ../src/uiobjects/uispectrum.cpp

${OBJECTDIR}/_ext/20f55786/uitextelementsarea.o: ../src/uiobjects/uitextelementsarea.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uitextelementsarea.o ../src/uiobjects/uitextelementsarea.cpp

${OBJECTDIR}/_ext/1ab3761f/unziptool.o: ../src/uiobjects/unzip/unziptool.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/1ab3761f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../src/httpcurl -I../src/rijndael -I../src/uiobjects -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1ab3761f/unziptool.o ../src/uiobjects/unzip/unziptool.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
