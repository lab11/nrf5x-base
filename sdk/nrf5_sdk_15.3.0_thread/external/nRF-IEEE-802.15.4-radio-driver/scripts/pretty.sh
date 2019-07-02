#!/usr/bin/env bash
#
# Copyright (c) 2018, Nordic Semiconductor ASA
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of source code must retain the above copyright notice, this
#      list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#
#   3. Neither the name of Nordic Semiconductor ASA nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
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
#

set -e
shopt -s globstar

##################################################################################
## Configuration
##################################################################################

CODE_PATH="./src"

if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    UNCRUSTIFY_BIN="$(which uncrustify)"
else
    UNCRUSTIFY_BIN="$(where uncrustify)"
fi

UNCRUSTIFY_CFG=".uncrustify.cfg"
UNCRUSTIFY_LANG="C"

if [[ -n "$1" && $1 = "check" ]]; then UNCRUSTIFY_CHECK=1; fi

##################################################################################
## Execution
##################################################################################

# Check if binary has been found
$UNCRUSTIFY_BIN --version || die

for file in $CODE_PATH/**/*
do
    if [ -f "$file" ];
    then
        if [ -n "$UNCRUSTIFY_CHECK" ];
        then
            $UNCRUSTIFY_BIN -c $UNCRUSTIFY_CFG -l $UNCRUSTIFY_LANG -q -f "$file" | diff -u "$file" - || pretty_check_fail=1
        else
            $UNCRUSTIFY_BIN -c $UNCRUSTIFY_CFG -l $UNCRUSTIFY_LANG --no-backup "$file"
        fi
    fi
done

if [ -n "$pretty_check_fail" ];
then
    echo "ERROR: Code style issues detected. Please make sure to run ./scripts/pretty.sh script."
    exit 1
else
    echo "SUCCESS"
fi
