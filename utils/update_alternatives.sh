#!/bin/bash
#
CMAKE_INSTALL_PREFIX="$1"
DESTINATION="$2"
LIB="$3"
CMAKE_SHARED_LIBRARY_SUFFIX="$4"
SCIDB_VERSION_MAJOR="$5"
SCIDB_VERSION_MINOR="$6"
SCIDB_P4="$7"
if [ "$SCIDB_P4" = "scidb" ]; then
    PRIORITY=10
else
    PRIORITY=20
fi

mkdir -p ${CMAKE_INSTALL_PREFIX}/alternatives
mkdir -p ${CMAKE_INSTALL_PREFIX}/admindir

update-alternatives --altdir ${CMAKE_INSTALL_PREFIX}/alternatives --admindir ${CMAKE_INSTALL_PREFIX}/admindir --install \
${CMAKE_INSTALL_PREFIX}/${DESTINATION}/lib${LIB}${CMAKE_SHARED_LIBRARY_SUFFIX} \
${LIB}.${SCIDB_VERSION_MAJOR}.${SCIDB_VERSION_MINOR} \
${CMAKE_INSTALL_PREFIX}/${DESTINATION}/lib${LIB}-${SCIDB_P4}${CMAKE_SHARED_LIBRARY_SUFFIX} \
${PRIORITY}
