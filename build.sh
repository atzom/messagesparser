# Copyright (c) 2015, Andreas Tzomakas
#  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of messagesparser nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#!/bin/bash

_library_dir="library/"
_enable_debug=""

if [ "$1" = "debug" ]; then
    _enable_debug='-g -O0'
fi

test -f ${_library_dir}/options.o && rm -f ${_library_dir}/options.o
test -f ${_library_dir}/log.o && rm -f ${_library_dir}/log.o
test -f ${_library_dir}/parser.o && rm -f ${_library_dir}/parser.o
test -f ${_library_dir}/msg.o && rm -f ${_library_dir}/msg.o
test -f ${_library_dir}/output.o && rm -f ${_library_dir}/output.o
test -f ${_library_dir}/xml.o && rm -f ${_library_dir}/xml.o
test -f ${_library_dir}/text.o && rm -f ${_library_dir}/text.o
test -f ${_library_dir}/stdout.o && rm -f ${_library_dir}/stdout.o
test -f ${_library_dir}/msg_logs && rm -f ${_library_dir}/msg_logs
test -f ${_library_dir}/strutil.o && rm -f ${_library_dir}/strutil.o
test -f ${_library_dir}/config.o && rm -f ${_library_dir}/config.o
test -f ${_library_dir}/messagesparser && rm -f ${_library_dir}/messagesparser

echo "Building the library..."
g++ ${_library_dir}/config.cpp -o ${_library_dir}/config.o -c ${_enable_debug} && \
g++ ${_library_dir}/strutil.cpp -o ${_library_dir}/strutil.o -c ${_enable_debug} && \
g++ ${_library_dir}/options.cpp -o ${_library_dir}/options.o -c ${_enable_debug} && \
g++ ${_library_dir}/log.cpp -o ${_library_dir}/log.o -c ${_enable_debug} && \
g++ ${_library_dir}/parser.cpp -o ${_library_dir}/parser.o -c ${_enable_debug} && \
g++ ${_library_dir}/msg.cpp -o ${_library_dir}/msg.o -c ${_enable_debug} && \
g++ ${_library_dir}/output.cpp -o ${_library_dir}/output.o -c ${_enable_debug} && \
g++ ${_library_dir}/xml.cpp -o ${_library_dir}/xml.o -c ${_enable_debug} && \
g++ ${_library_dir}/text.cpp -o ${_library_dir}/text.o -c ${_enable_debug} && \
g++ ${_library_dir}/stdout.cpp -o ${_library_dir}/stdout.o -c ${_enable_debug} && \
echo "Building main application ..." && \
g++ main.cpp ${_enable_debug} -o messagesparser \
 ${_library_dir}/parser.o \
 ${_library_dir}/msg.o \
 ${_library_dir}/log.o \
 ${_library_dir}/output.o \
 ${_library_dir}/options.o \
 ${_library_dir}/strutil.o \
 ${_library_dir}/xml.o \
 ${_library_dir}/text.o \
 ${_library_dir}/stdout.o \
 ${_library_dir}/config.o && \
echo "Builded successfully" || \
echo "Build failed"

rm -f ${_library_dir}/*.o
