#!/bin/bash
# Simple snipper to create the [buttons] section for qmice-map.ini
# By José Puga 2023. MIT License.

echo "[buttons]"
# This file is part of the linux kernel headers
grep BTN_ /usr/include/linux/input-event-codes.h | \
    # Cut spaces is not necessary#
	tr -d '\t' | \
	sed 's@#define @#@; s@0x@= @' | \
	cut -f1 -d" "
