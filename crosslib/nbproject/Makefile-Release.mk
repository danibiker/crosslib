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
CND_PLATFORM=MinGW_64-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/45e9766a/attribute.o \
	${OBJECTDIR}/_ext/45e9766a/char_ref.o \
	${OBJECTDIR}/_ext/45e9766a/error.o \
	${OBJECTDIR}/_ext/45e9766a/parser.o \
	${OBJECTDIR}/_ext/45e9766a/string_buffer.o \
	${OBJECTDIR}/_ext/45e9766a/string_piece.o \
	${OBJECTDIR}/_ext/45e9766a/tag.o \
	${OBJECTDIR}/_ext/45e9766a/tokenizer.o \
	${OBJECTDIR}/_ext/45e9766a/utf8.o \
	${OBJECTDIR}/_ext/45e9766a/util.o \
	${OBJECTDIR}/_ext/45e9766a/vector.o \
	${OBJECTDIR}/_ext/64c357a0/freedb.o \
	${OBJECTDIR}/_ext/f109d26e/htmlparser.o \
	${OBJECTDIR}/_ext/f109d26e/httputil.o \
	${OBJECTDIR}/_ext/3400367a/uiimgdownloader.o \
	${OBJECTDIR}/_ext/3400367a/uiimgencoder.o \
	${OBJECTDIR}/_ext/dfa85205/json_reader.o \
	${OBJECTDIR}/_ext/dfa85205/json_value.o \
	${OBJECTDIR}/_ext/dfa85205/json_writer.o \
	${OBJECTDIR}/_ext/51d5feb5/MusixMatch.o \
	${OBJECTDIR}/_ext/51d5feb5/darklyrics.o \
	${OBJECTDIR}/_ext/51d5feb5/lyricsbase.o \
	${OBJECTDIR}/_ext/51d5feb5/lyricswikia.o \
	${OBJECTDIR}/_ext/391a7aef/allmyvideos.o \
	${OBJECTDIR}/_ext/391a7aef/dropbox.o \
	${OBJECTDIR}/_ext/391a7aef/googledrive.o \
	${OBJECTDIR}/_ext/391a7aef/ioauth2.o \
	${OBJECTDIR}/_ext/391a7aef/streamcloud.o \
	${OBJECTDIR}/_ext/391a7aef/vk.o \
	${OBJECTDIR}/_ext/749d5e76/tinystr.o \
	${OBJECTDIR}/_ext/749d5e76/tinyxml.o \
	${OBJECTDIR}/_ext/749d5e76/tinyxmlerror.o \
	${OBJECTDIR}/_ext/749d5e76/tinyxmlparser.o \
	${OBJECTDIR}/_ext/64cbc303/ConstantHttp.o \
	${OBJECTDIR}/_ext/f9265173/cdjpeg.o \
	${OBJECTDIR}/_ext/f9265173/example.o \
	${OBJECTDIR}/_ext/f9265173/jcapimin.o \
	${OBJECTDIR}/_ext/f9265173/jcapistd.o \
	${OBJECTDIR}/_ext/f9265173/jccoefct.o \
	${OBJECTDIR}/_ext/f9265173/jccolor.o \
	${OBJECTDIR}/_ext/f9265173/jcdctmgr.o \
	${OBJECTDIR}/_ext/f9265173/jchuff.o \
	${OBJECTDIR}/_ext/f9265173/jcinit.o \
	${OBJECTDIR}/_ext/f9265173/jcmainct.o \
	${OBJECTDIR}/_ext/f9265173/jcmarker.o \
	${OBJECTDIR}/_ext/f9265173/jcmaster.o \
	${OBJECTDIR}/_ext/f9265173/jcomapi.o \
	${OBJECTDIR}/_ext/f9265173/jcparam.o \
	${OBJECTDIR}/_ext/f9265173/jcphuff.o \
	${OBJECTDIR}/_ext/f9265173/jcprepct.o \
	${OBJECTDIR}/_ext/f9265173/jcsample.o \
	${OBJECTDIR}/_ext/f9265173/jctrans.o \
	${OBJECTDIR}/_ext/f9265173/jdapimin.o \
	${OBJECTDIR}/_ext/f9265173/jdapistd.o \
	${OBJECTDIR}/_ext/f9265173/jdatadst.o \
	${OBJECTDIR}/_ext/f9265173/jdatasrc.o \
	${OBJECTDIR}/_ext/f9265173/jdcoefct.o \
	${OBJECTDIR}/_ext/f9265173/jdcolor.o \
	${OBJECTDIR}/_ext/f9265173/jddctmgr.o \
	${OBJECTDIR}/_ext/f9265173/jdhuff.o \
	${OBJECTDIR}/_ext/f9265173/jdinput.o \
	${OBJECTDIR}/_ext/f9265173/jdmainct.o \
	${OBJECTDIR}/_ext/f9265173/jdmarker.o \
	${OBJECTDIR}/_ext/f9265173/jdmaster.o \
	${OBJECTDIR}/_ext/f9265173/jdmerge.o \
	${OBJECTDIR}/_ext/f9265173/jdphuff.o \
	${OBJECTDIR}/_ext/f9265173/jdpostct.o \
	${OBJECTDIR}/_ext/f9265173/jdsample.o \
	${OBJECTDIR}/_ext/f9265173/jdtrans.o \
	${OBJECTDIR}/_ext/f9265173/jerror.o \
	${OBJECTDIR}/_ext/f9265173/jfdctflt.o \
	${OBJECTDIR}/_ext/f9265173/jfdctfst.o \
	${OBJECTDIR}/_ext/f9265173/jfdctint.o \
	${OBJECTDIR}/_ext/f9265173/jidctflt.o \
	${OBJECTDIR}/_ext/f9265173/jidctfst.o \
	${OBJECTDIR}/_ext/f9265173/jidctint.o \
	${OBJECTDIR}/_ext/f9265173/jidctred.o \
	${OBJECTDIR}/_ext/f9265173/jmemansi.o \
	${OBJECTDIR}/_ext/f9265173/jmemmgr.o \
	${OBJECTDIR}/_ext/f9265173/jquant1.o \
	${OBJECTDIR}/_ext/f9265173/jquant2.o \
	${OBJECTDIR}/_ext/f9265173/jutils.o \
	${OBJECTDIR}/_ext/f9265173/rdbmp.o \
	${OBJECTDIR}/_ext/f9265173/rdcolmap.o \
	${OBJECTDIR}/_ext/f9265173/rdgif.o \
	${OBJECTDIR}/_ext/f9265173/rdppm.o \
	${OBJECTDIR}/_ext/f9265173/rdrle.o \
	${OBJECTDIR}/_ext/f9265173/rdswitch.o \
	${OBJECTDIR}/_ext/f9265173/rdtarga.o \
	${OBJECTDIR}/_ext/f9265173/transupp.o \
	${OBJECTDIR}/_ext/f9265173/wrbmp.o \
	${OBJECTDIR}/_ext/f9265173/wrgif.o \
	${OBJECTDIR}/_ext/f9265173/wrppm.o \
	${OBJECTDIR}/_ext/f9265173/wrrle.o \
	${OBJECTDIR}/_ext/f9265173/wrtarga.o \
	${OBJECTDIR}/_ext/a8c489d9/filecipher.o \
	${OBJECTDIR}/_ext/a8c489d9/rijndael.o \
	${OBJECTDIR}/_ext/a8c489d9/sha256.o \
	${OBJECTDIR}/_ext/79a67082/sqlite3.o \
	${OBJECTDIR}/_ext/6eccffd9/access.o \
	${OBJECTDIR}/_ext/6eccffd9/alloc.o \
	${OBJECTDIR}/_ext/6eccffd9/attrask.o \
	${OBJECTDIR}/_ext/6eccffd9/attrdict.o \
	${OBJECTDIR}/_ext/6eccffd9/attrget.o \
	${OBJECTDIR}/_ext/6eccffd9/attrs.o \
	${OBJECTDIR}/_ext/6eccffd9/buffio.o \
	${OBJECTDIR}/_ext/6eccffd9/charsets.o \
	${OBJECTDIR}/_ext/6eccffd9/clean.o \
	${OBJECTDIR}/_ext/6eccffd9/config.o \
	${OBJECTDIR}/_ext/6eccffd9/entities.o \
	${OBJECTDIR}/_ext/6eccffd9/fileio.o \
	${OBJECTDIR}/_ext/6eccffd9/iconvtc.o \
	${OBJECTDIR}/_ext/6eccffd9/istack.o \
	${OBJECTDIR}/_ext/6eccffd9/lexer.o \
	${OBJECTDIR}/_ext/6eccffd9/localize.o \
	${OBJECTDIR}/_ext/6eccffd9/mappedio.o \
	${OBJECTDIR}/_ext/6eccffd9/parser.o \
	${OBJECTDIR}/_ext/6eccffd9/pprint.o \
	${OBJECTDIR}/_ext/6eccffd9/streamio.o \
	${OBJECTDIR}/_ext/6eccffd9/tagask.o \
	${OBJECTDIR}/_ext/6eccffd9/tags.o \
	${OBJECTDIR}/_ext/6eccffd9/tidylib.o \
	${OBJECTDIR}/_ext/6eccffd9/tmbstr.o \
	${OBJECTDIR}/_ext/6eccffd9/utf8.o \
	${OBJECTDIR}/_ext/6eccffd9/win32tc.o \
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
	${OBJECTDIR}/_ext/8d36c5a9/Database.o \
	${OBJECTDIR}/_ext/19a342fa/listgroupcol.o \
	${OBJECTDIR}/_ext/20f55786/cursor.o \
	${OBJECTDIR}/_ext/20f55786/filelaunch.o \
	${OBJECTDIR}/_ext/20f55786/joymapper.o \
	${OBJECTDIR}/_ext/20f55786/object.o \
	${OBJECTDIR}/_ext/20f55786/registry.o \
	${OBJECTDIR}/_ext/fd9e4b8e/tinystr.o \
	${OBJECTDIR}/_ext/fd9e4b8e/tinyxml.o \
	${OBJECTDIR}/_ext/fd9e4b8e/tinyxmlerror.o \
	${OBJECTDIR}/_ext/fd9e4b8e/tinyxmlparser.o \
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
	${OBJECTDIR}/_ext/1ab3761f/unziptool.o \
	${OBJECTDIR}/_ext/e901d183/ioapi.o \
	${OBJECTDIR}/_ext/e901d183/unzip.o \
	${OBJECTDIR}/_ext/e901d183/unziptool.o \
	${OBJECTDIR}/_ext/fc87cd0/adler32.o \
	${OBJECTDIR}/_ext/fc87cd0/compress.o \
	${OBJECTDIR}/_ext/fc87cd0/crc32.o \
	${OBJECTDIR}/_ext/fc87cd0/deflate.o \
	${OBJECTDIR}/_ext/fc87cd0/gzio.o \
	${OBJECTDIR}/_ext/fc87cd0/infback.o \
	${OBJECTDIR}/_ext/fc87cd0/inffast.o \
	${OBJECTDIR}/_ext/fc87cd0/inflate.o \
	${OBJECTDIR}/_ext/fc87cd0/inftrees.o \
	${OBJECTDIR}/_ext/fc87cd0/trees.o \
	${OBJECTDIR}/_ext/fc87cd0/uncompr.o \
	${OBJECTDIR}/_ext/fc87cd0/zutil.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

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

${OBJECTDIR}/_ext/45e9766a/attribute.o: ../src/gumbo-parser-master/src/attribute.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/attribute.o ../src/gumbo-parser-master/src/attribute.c

${OBJECTDIR}/_ext/45e9766a/char_ref.o: ../src/gumbo-parser-master/src/char_ref.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/char_ref.o ../src/gumbo-parser-master/src/char_ref.c

${OBJECTDIR}/_ext/45e9766a/error.o: ../src/gumbo-parser-master/src/error.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/error.o ../src/gumbo-parser-master/src/error.c

${OBJECTDIR}/_ext/45e9766a/parser.o: ../src/gumbo-parser-master/src/parser.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/parser.o ../src/gumbo-parser-master/src/parser.c

${OBJECTDIR}/_ext/45e9766a/string_buffer.o: ../src/gumbo-parser-master/src/string_buffer.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/string_buffer.o ../src/gumbo-parser-master/src/string_buffer.c

${OBJECTDIR}/_ext/45e9766a/string_piece.o: ../src/gumbo-parser-master/src/string_piece.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/string_piece.o ../src/gumbo-parser-master/src/string_piece.c

${OBJECTDIR}/_ext/45e9766a/tag.o: ../src/gumbo-parser-master/src/tag.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/tag.o ../src/gumbo-parser-master/src/tag.c

${OBJECTDIR}/_ext/45e9766a/tokenizer.o: ../src/gumbo-parser-master/src/tokenizer.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/tokenizer.o ../src/gumbo-parser-master/src/tokenizer.c

${OBJECTDIR}/_ext/45e9766a/utf8.o: ../src/gumbo-parser-master/src/utf8.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/utf8.o ../src/gumbo-parser-master/src/utf8.c

${OBJECTDIR}/_ext/45e9766a/util.o: ../src/gumbo-parser-master/src/util.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/util.o ../src/gumbo-parser-master/src/util.c

${OBJECTDIR}/_ext/45e9766a/vector.o: ../src/gumbo-parser-master/src/vector.c
	${MKDIR} -p ${OBJECTDIR}/_ext/45e9766a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/45e9766a/vector.o ../src/gumbo-parser-master/src/vector.c

${OBJECTDIR}/_ext/64c357a0/freedb.o: ../src/httpcurl/cddb/freedb.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/64c357a0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/64c357a0/freedb.o ../src/httpcurl/cddb/freedb.cpp

${OBJECTDIR}/_ext/f109d26e/htmlparser.o: ../src/httpcurl/htmlparser.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f109d26e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f109d26e/htmlparser.o ../src/httpcurl/htmlparser.cpp

${OBJECTDIR}/_ext/f109d26e/httputil.o: ../src/httpcurl/httputil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f109d26e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f109d26e/httputil.o ../src/httpcurl/httputil.cpp

${OBJECTDIR}/_ext/3400367a/uiimgdownloader.o: ../src/httpcurl/image/uiimgdownloader.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/3400367a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3400367a/uiimgdownloader.o ../src/httpcurl/image/uiimgdownloader.cpp

${OBJECTDIR}/_ext/3400367a/uiimgencoder.o: ../src/httpcurl/image/uiimgencoder.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/3400367a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3400367a/uiimgencoder.o ../src/httpcurl/image/uiimgencoder.cpp

${OBJECTDIR}/_ext/dfa85205/json_reader.o: ../src/httpcurl/jsoncpp-0.10.5/src/lib_json/json_reader.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dfa85205
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dfa85205/json_reader.o ../src/httpcurl/jsoncpp-0.10.5/src/lib_json/json_reader.cpp

${OBJECTDIR}/_ext/dfa85205/json_value.o: ../src/httpcurl/jsoncpp-0.10.5/src/lib_json/json_value.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dfa85205
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dfa85205/json_value.o ../src/httpcurl/jsoncpp-0.10.5/src/lib_json/json_value.cpp

${OBJECTDIR}/_ext/dfa85205/json_writer.o: ../src/httpcurl/jsoncpp-0.10.5/src/lib_json/json_writer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dfa85205
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dfa85205/json_writer.o ../src/httpcurl/jsoncpp-0.10.5/src/lib_json/json_writer.cpp

${OBJECTDIR}/_ext/51d5feb5/MusixMatch.o: ../src/httpcurl/lyrics/MusixMatch.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/MusixMatch.o ../src/httpcurl/lyrics/MusixMatch.cpp

${OBJECTDIR}/_ext/51d5feb5/darklyrics.o: ../src/httpcurl/lyrics/darklyrics.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/darklyrics.o ../src/httpcurl/lyrics/darklyrics.cpp

${OBJECTDIR}/_ext/51d5feb5/lyricsbase.o: ../src/httpcurl/lyrics/lyricsbase.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/lyricsbase.o ../src/httpcurl/lyrics/lyricsbase.cpp

${OBJECTDIR}/_ext/51d5feb5/lyricswikia.o: ../src/httpcurl/lyrics/lyricswikia.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/51d5feb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/51d5feb5/lyricswikia.o ../src/httpcurl/lyrics/lyricswikia.cpp

${OBJECTDIR}/_ext/391a7aef/allmyvideos.o: ../src/httpcurl/servers/allmyvideos.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/allmyvideos.o ../src/httpcurl/servers/allmyvideos.cpp

${OBJECTDIR}/_ext/391a7aef/dropbox.o: ../src/httpcurl/servers/dropbox.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/dropbox.o ../src/httpcurl/servers/dropbox.cpp

${OBJECTDIR}/_ext/391a7aef/googledrive.o: ../src/httpcurl/servers/googledrive.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/googledrive.o ../src/httpcurl/servers/googledrive.cpp

${OBJECTDIR}/_ext/391a7aef/ioauth2.o: ../src/httpcurl/servers/ioauth2.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/ioauth2.o ../src/httpcurl/servers/ioauth2.cpp

${OBJECTDIR}/_ext/391a7aef/streamcloud.o: ../src/httpcurl/servers/streamcloud.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/streamcloud.o ../src/httpcurl/servers/streamcloud.cpp

${OBJECTDIR}/_ext/391a7aef/vk.o: ../src/httpcurl/servers/vk.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/391a7aef
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/391a7aef/vk.o ../src/httpcurl/servers/vk.cpp

${OBJECTDIR}/_ext/749d5e76/tinystr.o: ../src/httpcurl/tinyxml/tinystr.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/749d5e76
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749d5e76/tinystr.o ../src/httpcurl/tinyxml/tinystr.cpp

${OBJECTDIR}/_ext/749d5e76/tinyxml.o: ../src/httpcurl/tinyxml/tinyxml.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/749d5e76
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749d5e76/tinyxml.o ../src/httpcurl/tinyxml/tinyxml.cpp

${OBJECTDIR}/_ext/749d5e76/tinyxmlerror.o: ../src/httpcurl/tinyxml/tinyxmlerror.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/749d5e76
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749d5e76/tinyxmlerror.o ../src/httpcurl/tinyxml/tinyxmlerror.cpp

${OBJECTDIR}/_ext/749d5e76/tinyxmlparser.o: ../src/httpcurl/tinyxml/tinyxmlparser.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/749d5e76
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749d5e76/tinyxmlparser.o ../src/httpcurl/tinyxml/tinyxmlparser.cpp

${OBJECTDIR}/_ext/64cbc303/ConstantHttp.o: ../src/httpcurl/util/ConstantHttp.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/64cbc303
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/64cbc303/ConstantHttp.o ../src/httpcurl/util/ConstantHttp.cpp

${OBJECTDIR}/_ext/f9265173/cdjpeg.o: ../src/libjpeg/cdjpeg.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/cdjpeg.o ../src/libjpeg/cdjpeg.c

${OBJECTDIR}/_ext/f9265173/example.o: ../src/libjpeg/example.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/example.o ../src/libjpeg/example.c

${OBJECTDIR}/_ext/f9265173/jcapimin.o: ../src/libjpeg/jcapimin.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcapimin.o ../src/libjpeg/jcapimin.c

${OBJECTDIR}/_ext/f9265173/jcapistd.o: ../src/libjpeg/jcapistd.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcapistd.o ../src/libjpeg/jcapistd.c

${OBJECTDIR}/_ext/f9265173/jccoefct.o: ../src/libjpeg/jccoefct.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jccoefct.o ../src/libjpeg/jccoefct.c

${OBJECTDIR}/_ext/f9265173/jccolor.o: ../src/libjpeg/jccolor.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jccolor.o ../src/libjpeg/jccolor.c

${OBJECTDIR}/_ext/f9265173/jcdctmgr.o: ../src/libjpeg/jcdctmgr.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcdctmgr.o ../src/libjpeg/jcdctmgr.c

${OBJECTDIR}/_ext/f9265173/jchuff.o: ../src/libjpeg/jchuff.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jchuff.o ../src/libjpeg/jchuff.c

${OBJECTDIR}/_ext/f9265173/jcinit.o: ../src/libjpeg/jcinit.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcinit.o ../src/libjpeg/jcinit.c

${OBJECTDIR}/_ext/f9265173/jcmainct.o: ../src/libjpeg/jcmainct.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcmainct.o ../src/libjpeg/jcmainct.c

${OBJECTDIR}/_ext/f9265173/jcmarker.o: ../src/libjpeg/jcmarker.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcmarker.o ../src/libjpeg/jcmarker.c

${OBJECTDIR}/_ext/f9265173/jcmaster.o: ../src/libjpeg/jcmaster.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcmaster.o ../src/libjpeg/jcmaster.c

${OBJECTDIR}/_ext/f9265173/jcomapi.o: ../src/libjpeg/jcomapi.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcomapi.o ../src/libjpeg/jcomapi.c

${OBJECTDIR}/_ext/f9265173/jcparam.o: ../src/libjpeg/jcparam.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcparam.o ../src/libjpeg/jcparam.c

${OBJECTDIR}/_ext/f9265173/jcphuff.o: ../src/libjpeg/jcphuff.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcphuff.o ../src/libjpeg/jcphuff.c

${OBJECTDIR}/_ext/f9265173/jcprepct.o: ../src/libjpeg/jcprepct.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcprepct.o ../src/libjpeg/jcprepct.c

${OBJECTDIR}/_ext/f9265173/jcsample.o: ../src/libjpeg/jcsample.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jcsample.o ../src/libjpeg/jcsample.c

${OBJECTDIR}/_ext/f9265173/jctrans.o: ../src/libjpeg/jctrans.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jctrans.o ../src/libjpeg/jctrans.c

${OBJECTDIR}/_ext/f9265173/jdapimin.o: ../src/libjpeg/jdapimin.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdapimin.o ../src/libjpeg/jdapimin.c

${OBJECTDIR}/_ext/f9265173/jdapistd.o: ../src/libjpeg/jdapistd.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdapistd.o ../src/libjpeg/jdapistd.c

${OBJECTDIR}/_ext/f9265173/jdatadst.o: ../src/libjpeg/jdatadst.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdatadst.o ../src/libjpeg/jdatadst.c

${OBJECTDIR}/_ext/f9265173/jdatasrc.o: ../src/libjpeg/jdatasrc.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdatasrc.o ../src/libjpeg/jdatasrc.c

${OBJECTDIR}/_ext/f9265173/jdcoefct.o: ../src/libjpeg/jdcoefct.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdcoefct.o ../src/libjpeg/jdcoefct.c

${OBJECTDIR}/_ext/f9265173/jdcolor.o: ../src/libjpeg/jdcolor.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdcolor.o ../src/libjpeg/jdcolor.c

${OBJECTDIR}/_ext/f9265173/jddctmgr.o: ../src/libjpeg/jddctmgr.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jddctmgr.o ../src/libjpeg/jddctmgr.c

${OBJECTDIR}/_ext/f9265173/jdhuff.o: ../src/libjpeg/jdhuff.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdhuff.o ../src/libjpeg/jdhuff.c

${OBJECTDIR}/_ext/f9265173/jdinput.o: ../src/libjpeg/jdinput.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdinput.o ../src/libjpeg/jdinput.c

${OBJECTDIR}/_ext/f9265173/jdmainct.o: ../src/libjpeg/jdmainct.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdmainct.o ../src/libjpeg/jdmainct.c

${OBJECTDIR}/_ext/f9265173/jdmarker.o: ../src/libjpeg/jdmarker.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdmarker.o ../src/libjpeg/jdmarker.c

${OBJECTDIR}/_ext/f9265173/jdmaster.o: ../src/libjpeg/jdmaster.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdmaster.o ../src/libjpeg/jdmaster.c

${OBJECTDIR}/_ext/f9265173/jdmerge.o: ../src/libjpeg/jdmerge.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdmerge.o ../src/libjpeg/jdmerge.c

${OBJECTDIR}/_ext/f9265173/jdphuff.o: ../src/libjpeg/jdphuff.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdphuff.o ../src/libjpeg/jdphuff.c

${OBJECTDIR}/_ext/f9265173/jdpostct.o: ../src/libjpeg/jdpostct.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdpostct.o ../src/libjpeg/jdpostct.c

${OBJECTDIR}/_ext/f9265173/jdsample.o: ../src/libjpeg/jdsample.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdsample.o ../src/libjpeg/jdsample.c

${OBJECTDIR}/_ext/f9265173/jdtrans.o: ../src/libjpeg/jdtrans.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jdtrans.o ../src/libjpeg/jdtrans.c

${OBJECTDIR}/_ext/f9265173/jerror.o: ../src/libjpeg/jerror.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jerror.o ../src/libjpeg/jerror.c

${OBJECTDIR}/_ext/f9265173/jfdctflt.o: ../src/libjpeg/jfdctflt.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jfdctflt.o ../src/libjpeg/jfdctflt.c

${OBJECTDIR}/_ext/f9265173/jfdctfst.o: ../src/libjpeg/jfdctfst.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jfdctfst.o ../src/libjpeg/jfdctfst.c

${OBJECTDIR}/_ext/f9265173/jfdctint.o: ../src/libjpeg/jfdctint.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jfdctint.o ../src/libjpeg/jfdctint.c

${OBJECTDIR}/_ext/f9265173/jidctflt.o: ../src/libjpeg/jidctflt.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jidctflt.o ../src/libjpeg/jidctflt.c

${OBJECTDIR}/_ext/f9265173/jidctfst.o: ../src/libjpeg/jidctfst.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jidctfst.o ../src/libjpeg/jidctfst.c

${OBJECTDIR}/_ext/f9265173/jidctint.o: ../src/libjpeg/jidctint.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jidctint.o ../src/libjpeg/jidctint.c

${OBJECTDIR}/_ext/f9265173/jidctred.o: ../src/libjpeg/jidctred.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jidctred.o ../src/libjpeg/jidctred.c

${OBJECTDIR}/_ext/f9265173/jmemansi.o: ../src/libjpeg/jmemansi.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jmemansi.o ../src/libjpeg/jmemansi.c

${OBJECTDIR}/_ext/f9265173/jmemmgr.o: ../src/libjpeg/jmemmgr.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jmemmgr.o ../src/libjpeg/jmemmgr.c

${OBJECTDIR}/_ext/f9265173/jquant1.o: ../src/libjpeg/jquant1.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jquant1.o ../src/libjpeg/jquant1.c

${OBJECTDIR}/_ext/f9265173/jquant2.o: ../src/libjpeg/jquant2.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jquant2.o ../src/libjpeg/jquant2.c

${OBJECTDIR}/_ext/f9265173/jutils.o: ../src/libjpeg/jutils.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/jutils.o ../src/libjpeg/jutils.c

${OBJECTDIR}/_ext/f9265173/rdbmp.o: ../src/libjpeg/rdbmp.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdbmp.o ../src/libjpeg/rdbmp.c

${OBJECTDIR}/_ext/f9265173/rdcolmap.o: ../src/libjpeg/rdcolmap.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdcolmap.o ../src/libjpeg/rdcolmap.c

${OBJECTDIR}/_ext/f9265173/rdgif.o: ../src/libjpeg/rdgif.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdgif.o ../src/libjpeg/rdgif.c

${OBJECTDIR}/_ext/f9265173/rdppm.o: ../src/libjpeg/rdppm.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdppm.o ../src/libjpeg/rdppm.c

${OBJECTDIR}/_ext/f9265173/rdrle.o: ../src/libjpeg/rdrle.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdrle.o ../src/libjpeg/rdrle.c

${OBJECTDIR}/_ext/f9265173/rdswitch.o: ../src/libjpeg/rdswitch.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdswitch.o ../src/libjpeg/rdswitch.c

${OBJECTDIR}/_ext/f9265173/rdtarga.o: ../src/libjpeg/rdtarga.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/rdtarga.o ../src/libjpeg/rdtarga.c

${OBJECTDIR}/_ext/f9265173/transupp.o: ../src/libjpeg/transupp.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/transupp.o ../src/libjpeg/transupp.c

${OBJECTDIR}/_ext/f9265173/wrbmp.o: ../src/libjpeg/wrbmp.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/wrbmp.o ../src/libjpeg/wrbmp.c

${OBJECTDIR}/_ext/f9265173/wrgif.o: ../src/libjpeg/wrgif.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/wrgif.o ../src/libjpeg/wrgif.c

${OBJECTDIR}/_ext/f9265173/wrppm.o: ../src/libjpeg/wrppm.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/wrppm.o ../src/libjpeg/wrppm.c

${OBJECTDIR}/_ext/f9265173/wrrle.o: ../src/libjpeg/wrrle.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/wrrle.o ../src/libjpeg/wrrle.c

${OBJECTDIR}/_ext/f9265173/wrtarga.o: ../src/libjpeg/wrtarga.c
	${MKDIR} -p ${OBJECTDIR}/_ext/f9265173
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f9265173/wrtarga.o ../src/libjpeg/wrtarga.c

${OBJECTDIR}/_ext/a8c489d9/filecipher.o: ../src/rijndael/filecipher.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a8c489d9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a8c489d9/filecipher.o ../src/rijndael/filecipher.cpp

${OBJECTDIR}/_ext/a8c489d9/rijndael.o: ../src/rijndael/rijndael.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a8c489d9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a8c489d9/rijndael.o ../src/rijndael/rijndael.cpp

${OBJECTDIR}/_ext/a8c489d9/sha256.o: ../src/rijndael/sha256.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a8c489d9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a8c489d9/sha256.o ../src/rijndael/sha256.cpp

${OBJECTDIR}/_ext/79a67082/sqlite3.o: ../src/sqllite/sqlite3.c
	${MKDIR} -p ${OBJECTDIR}/_ext/79a67082
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/79a67082/sqlite3.o ../src/sqllite/sqlite3.c

${OBJECTDIR}/_ext/6eccffd9/access.o: ../src/tidy/src/access.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/access.o ../src/tidy/src/access.c

${OBJECTDIR}/_ext/6eccffd9/alloc.o: ../src/tidy/src/alloc.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/alloc.o ../src/tidy/src/alloc.c

${OBJECTDIR}/_ext/6eccffd9/attrask.o: ../src/tidy/src/attrask.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/attrask.o ../src/tidy/src/attrask.c

${OBJECTDIR}/_ext/6eccffd9/attrdict.o: ../src/tidy/src/attrdict.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/attrdict.o ../src/tidy/src/attrdict.c

${OBJECTDIR}/_ext/6eccffd9/attrget.o: ../src/tidy/src/attrget.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/attrget.o ../src/tidy/src/attrget.c

${OBJECTDIR}/_ext/6eccffd9/attrs.o: ../src/tidy/src/attrs.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/attrs.o ../src/tidy/src/attrs.c

${OBJECTDIR}/_ext/6eccffd9/buffio.o: ../src/tidy/src/buffio.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/buffio.o ../src/tidy/src/buffio.c

${OBJECTDIR}/_ext/6eccffd9/charsets.o: ../src/tidy/src/charsets.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/charsets.o ../src/tidy/src/charsets.c

${OBJECTDIR}/_ext/6eccffd9/clean.o: ../src/tidy/src/clean.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/clean.o ../src/tidy/src/clean.c

${OBJECTDIR}/_ext/6eccffd9/config.o: ../src/tidy/src/config.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/config.o ../src/tidy/src/config.c

${OBJECTDIR}/_ext/6eccffd9/entities.o: ../src/tidy/src/entities.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/entities.o ../src/tidy/src/entities.c

${OBJECTDIR}/_ext/6eccffd9/fileio.o: ../src/tidy/src/fileio.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/fileio.o ../src/tidy/src/fileio.c

${OBJECTDIR}/_ext/6eccffd9/iconvtc.o: ../src/tidy/src/iconvtc.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/iconvtc.o ../src/tidy/src/iconvtc.c

${OBJECTDIR}/_ext/6eccffd9/istack.o: ../src/tidy/src/istack.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/istack.o ../src/tidy/src/istack.c

${OBJECTDIR}/_ext/6eccffd9/lexer.o: ../src/tidy/src/lexer.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/lexer.o ../src/tidy/src/lexer.c

${OBJECTDIR}/_ext/6eccffd9/localize.o: ../src/tidy/src/localize.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/localize.o ../src/tidy/src/localize.c

${OBJECTDIR}/_ext/6eccffd9/mappedio.o: ../src/tidy/src/mappedio.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/mappedio.o ../src/tidy/src/mappedio.c

${OBJECTDIR}/_ext/6eccffd9/parser.o: ../src/tidy/src/parser.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/parser.o ../src/tidy/src/parser.c

${OBJECTDIR}/_ext/6eccffd9/pprint.o: ../src/tidy/src/pprint.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/pprint.o ../src/tidy/src/pprint.c

${OBJECTDIR}/_ext/6eccffd9/streamio.o: ../src/tidy/src/streamio.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/streamio.o ../src/tidy/src/streamio.c

${OBJECTDIR}/_ext/6eccffd9/tagask.o: ../src/tidy/src/tagask.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/tagask.o ../src/tidy/src/tagask.c

${OBJECTDIR}/_ext/6eccffd9/tags.o: ../src/tidy/src/tags.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/tags.o ../src/tidy/src/tags.c

${OBJECTDIR}/_ext/6eccffd9/tidylib.o: ../src/tidy/src/tidylib.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/tidylib.o ../src/tidy/src/tidylib.c

${OBJECTDIR}/_ext/6eccffd9/tmbstr.o: ../src/tidy/src/tmbstr.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/tmbstr.o ../src/tidy/src/tmbstr.c

${OBJECTDIR}/_ext/6eccffd9/utf8.o: ../src/tidy/src/utf8.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/utf8.o ../src/tidy/src/utf8.c

${OBJECTDIR}/_ext/6eccffd9/win32tc.o: ../src/tidy/src/win32tc.c
	${MKDIR} -p ${OBJECTDIR}/_ext/6eccffd9
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/6eccffd9/win32tc.o ../src/tidy/src/win32tc.c

${OBJECTDIR}/_ext/20f55786/Colorutil.o: ../src/uiobjects/Colorutil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Colorutil.o ../src/uiobjects/Colorutil.cpp

${OBJECTDIR}/_ext/20f55786/Constant.o: ../src/uiobjects/Constant.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Constant.o ../src/uiobjects/Constant.cpp

${OBJECTDIR}/_ext/20f55786/Dirutil.o: ../src/uiobjects/Dirutil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Dirutil.o ../src/uiobjects/Dirutil.cpp

${OBJECTDIR}/_ext/20f55786/Excepcion.o: ../src/uiobjects/Excepcion.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Excepcion.o ../src/uiobjects/Excepcion.cpp

${OBJECTDIR}/_ext/20f55786/Fileio.o: ../src/uiobjects/Fileio.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Fileio.o ../src/uiobjects/Fileio.cpp

${OBJECTDIR}/_ext/20f55786/ImagenGestor.o: ../src/uiobjects/ImagenGestor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/ImagenGestor.o ../src/uiobjects/ImagenGestor.cpp

${OBJECTDIR}/_ext/20f55786/Launcher.o: ../src/uiobjects/Launcher.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Launcher.o ../src/uiobjects/Launcher.cpp

${OBJECTDIR}/_ext/20f55786/ListaSimple.o: ../src/uiobjects/ListaSimple.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/ListaSimple.o ../src/uiobjects/ListaSimple.cpp

${OBJECTDIR}/_ext/20f55786/Menuobject.o: ../src/uiobjects/Menuobject.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Menuobject.o ../src/uiobjects/Menuobject.cpp

${OBJECTDIR}/_ext/20f55786/SHA1.o: ../src/uiobjects/SHA1.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/SHA1.o ../src/uiobjects/SHA1.cpp

${OBJECTDIR}/_ext/20f55786/Traza.o: ../src/uiobjects/Traza.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/Traza.o ../src/uiobjects/Traza.cpp

${OBJECTDIR}/_ext/8d36c5a9/Database.o: ../src/uiobjects/bbdd/Database.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/8d36c5a9
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/8d36c5a9/Database.o ../src/uiobjects/bbdd/Database.cpp

${OBJECTDIR}/_ext/19a342fa/listgroupcol.o: ../src/uiobjects/beans/listgroupcol.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/19a342fa
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/19a342fa/listgroupcol.o ../src/uiobjects/beans/listgroupcol.cpp

${OBJECTDIR}/_ext/20f55786/cursor.o: ../src/uiobjects/cursor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/cursor.o ../src/uiobjects/cursor.cpp

${OBJECTDIR}/_ext/20f55786/filelaunch.o: ../src/uiobjects/filelaunch.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/filelaunch.o ../src/uiobjects/filelaunch.cpp

${OBJECTDIR}/_ext/20f55786/joymapper.o: ../src/uiobjects/joymapper.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/joymapper.o ../src/uiobjects/joymapper.cpp

${OBJECTDIR}/_ext/20f55786/object.o: ../src/uiobjects/object.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/object.o ../src/uiobjects/object.cpp

${OBJECTDIR}/_ext/20f55786/registry.o: ../src/uiobjects/registry.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/registry.o ../src/uiobjects/registry.cpp

${OBJECTDIR}/_ext/fd9e4b8e/tinystr.o: ../src/uiobjects/tinyxml/tinystr.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/fd9e4b8e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fd9e4b8e/tinystr.o ../src/uiobjects/tinyxml/tinystr.cpp

${OBJECTDIR}/_ext/fd9e4b8e/tinyxml.o: ../src/uiobjects/tinyxml/tinyxml.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/fd9e4b8e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fd9e4b8e/tinyxml.o ../src/uiobjects/tinyxml/tinyxml.cpp

${OBJECTDIR}/_ext/fd9e4b8e/tinyxmlerror.o: ../src/uiobjects/tinyxml/tinyxmlerror.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/fd9e4b8e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fd9e4b8e/tinyxmlerror.o ../src/uiobjects/tinyxml/tinyxmlerror.cpp

${OBJECTDIR}/_ext/fd9e4b8e/tinyxmlparser.o: ../src/uiobjects/tinyxml/tinyxmlparser.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/fd9e4b8e
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fd9e4b8e/tinyxmlparser.o ../src/uiobjects/tinyxml/tinyxmlparser.cpp

${OBJECTDIR}/_ext/20f55786/uiart.o: ../src/uiobjects/uiart.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uiart.o ../src/uiobjects/uiart.cpp

${OBJECTDIR}/_ext/20f55786/uibutton.o: ../src/uiobjects/uibutton.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uibutton.o ../src/uiobjects/uibutton.cpp

${OBJECTDIR}/_ext/20f55786/uicheck.o: ../src/uiobjects/uicheck.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uicheck.o ../src/uiobjects/uicheck.cpp

${OBJECTDIR}/_ext/20f55786/uicombobox.o: ../src/uiobjects/uicombobox.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uicombobox.o ../src/uiobjects/uicombobox.cpp

${OBJECTDIR}/_ext/20f55786/uiinput.o: ../src/uiobjects/uiinput.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uiinput.o ../src/uiobjects/uiinput.cpp

${OBJECTDIR}/_ext/20f55786/uilabel.o: ../src/uiobjects/uilabel.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilabel.o ../src/uiobjects/uilabel.cpp

${OBJECTDIR}/_ext/20f55786/uilist.o: ../src/uiobjects/uilist.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilist.o ../src/uiobjects/uilist.cpp

${OBJECTDIR}/_ext/20f55786/uilistcommon.o: ../src/uiobjects/uilistcommon.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilistcommon.o ../src/uiobjects/uilistcommon.cpp

${OBJECTDIR}/_ext/20f55786/uilistgroup.o: ../src/uiobjects/uilistgroup.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uilistgroup.o ../src/uiobjects/uilistgroup.cpp

${OBJECTDIR}/_ext/20f55786/uipanel.o: ../src/uiobjects/uipanel.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipanel.o ../src/uiobjects/uipanel.cpp

${OBJECTDIR}/_ext/20f55786/uipanelborder.o: ../src/uiobjects/uipanelborder.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipanelborder.o ../src/uiobjects/uipanelborder.cpp

${OBJECTDIR}/_ext/20f55786/uipicture.o: ../src/uiobjects/uipicture.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipicture.o ../src/uiobjects/uipicture.cpp

${OBJECTDIR}/_ext/20f55786/uipopupmenu.o: ../src/uiobjects/uipopupmenu.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uipopupmenu.o ../src/uiobjects/uipopupmenu.cpp

${OBJECTDIR}/_ext/20f55786/uiprogressbar.o: ../src/uiobjects/uiprogressbar.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uiprogressbar.o ../src/uiobjects/uiprogressbar.cpp

${OBJECTDIR}/_ext/20f55786/uislider.o: ../src/uiobjects/uislider.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uislider.o ../src/uiobjects/uislider.cpp

${OBJECTDIR}/_ext/20f55786/uispectrum.o: ../src/uiobjects/uispectrum.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uispectrum.o ../src/uiobjects/uispectrum.cpp

${OBJECTDIR}/_ext/20f55786/uitextelementsarea.o: ../src/uiobjects/uitextelementsarea.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/20f55786
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/20f55786/uitextelementsarea.o ../src/uiobjects/uitextelementsarea.cpp

${OBJECTDIR}/_ext/1ab3761f/unziptool.o: ../src/uiobjects/unzip/unziptool.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/1ab3761f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1ab3761f/unziptool.o ../src/uiobjects/unzip/unziptool.cpp

${OBJECTDIR}/_ext/e901d183/ioapi.o: ../src/ziputils/unzip/ioapi.c
	${MKDIR} -p ${OBJECTDIR}/_ext/e901d183
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e901d183/ioapi.o ../src/ziputils/unzip/ioapi.c

${OBJECTDIR}/_ext/e901d183/unzip.o: ../src/ziputils/unzip/unzip.c
	${MKDIR} -p ${OBJECTDIR}/_ext/e901d183
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e901d183/unzip.o ../src/ziputils/unzip/unzip.c

${OBJECTDIR}/_ext/e901d183/unziptool.o: ../src/ziputils/unzip/unziptool.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e901d183
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DSKIP_HOSTNAME_VERIFICATION -DSKIP_PEER_VERIFICATION -DWIN -DWIN32 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e901d183/unziptool.o ../src/ziputils/unzip/unziptool.cpp

${OBJECTDIR}/_ext/fc87cd0/adler32.o: ../src/ziputils/zlib/adler32.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/adler32.o ../src/ziputils/zlib/adler32.c

${OBJECTDIR}/_ext/fc87cd0/compress.o: ../src/ziputils/zlib/compress.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/compress.o ../src/ziputils/zlib/compress.c

${OBJECTDIR}/_ext/fc87cd0/crc32.o: ../src/ziputils/zlib/crc32.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/crc32.o ../src/ziputils/zlib/crc32.c

${OBJECTDIR}/_ext/fc87cd0/deflate.o: ../src/ziputils/zlib/deflate.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/deflate.o ../src/ziputils/zlib/deflate.c

${OBJECTDIR}/_ext/fc87cd0/gzio.o: ../src/ziputils/zlib/gzio.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/gzio.o ../src/ziputils/zlib/gzio.c

${OBJECTDIR}/_ext/fc87cd0/infback.o: ../src/ziputils/zlib/infback.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/infback.o ../src/ziputils/zlib/infback.c

${OBJECTDIR}/_ext/fc87cd0/inffast.o: ../src/ziputils/zlib/inffast.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/inffast.o ../src/ziputils/zlib/inffast.c

${OBJECTDIR}/_ext/fc87cd0/inflate.o: ../src/ziputils/zlib/inflate.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/inflate.o ../src/ziputils/zlib/inflate.c

${OBJECTDIR}/_ext/fc87cd0/inftrees.o: ../src/ziputils/zlib/inftrees.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/inftrees.o ../src/ziputils/zlib/inftrees.c

${OBJECTDIR}/_ext/fc87cd0/trees.o: ../src/ziputils/zlib/trees.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/trees.o ../src/ziputils/zlib/trees.c

${OBJECTDIR}/_ext/fc87cd0/uncompr.o: ../src/ziputils/zlib/uncompr.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/uncompr.o ../src/ziputils/zlib/uncompr.c

${OBJECTDIR}/_ext/fc87cd0/zutil.o: ../src/ziputils/zlib/zutil.c
	${MKDIR} -p ${OBJECTDIR}/_ext/fc87cd0
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../curl-7.40.0-devel-mingw32/include -I../../SDL/libs/include -I../../SDL/libs/include/SDL -I../src/ziputils/unzip -I../src/ziputils/zlib -I../src/uiobjects -I../src/sqllite -I../src/rijndael -I../src/httpcurl -I../src/libjpeg -I../src/httpcurl/jsoncpp-0.10.5/include -I../src/tidy/include -I../src/gumbo-parser-master/src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fc87cd0/zutil.o ../src/ziputils/zlib/zutil.c

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
