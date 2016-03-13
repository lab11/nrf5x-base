#!/usr/bin/env bash

set -e

for d in ./*/ ; do
	pushd "$d"
	echo "Building $d App"
	make clean
	make
	popd
done
