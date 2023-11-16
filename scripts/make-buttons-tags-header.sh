#!/bin/bash
# Simple snipper to extract the mouse button codes.
# By José Puga 2023. MIT License.

# This file is part of the linux kernel headers
grep BTN_ /usr/include/linux/input-event-codes.h | \
    # Cut spaces is not necessary
	tr -d '\t' | \
    # Some lines contains comments like /* ... */
	cut -f1 -d"/" | \
	sed 's@#define @{"@; s@0x@", 0x@; s@$@},@'

