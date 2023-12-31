#ifndef B40DF103_2A4C_4229_A006_8225B2F06003
#define B40DF103_2A4C_4229_A006_8225B2F06003
/*
EXTRACTED FROM /usr/include/linux/input-event-codes.h with a bash script:
DOES NOT WORKS 100% Requires some manual fixeds

#!/bin/bash
# Simple snipper to extract the mouse button codes.
# By José Puga 2023. MIT License.

# This file is part of the linux kernel headers
grep BTN_ /usr/include/linux/input-event-codes.h | \
    # Cut spaces is not necessary
	tr -d '\t' | \
    # Some lines contains comments like / * ... * /
	cut -f1 -d"/" | \
	sed 's@#define @{"@; s@0x@", 0x@; s@$@},@'

*/
#include <map>
#include <string>

namespace buttons_tags {
static const std::map<std::string, int> tags {
    {"BTN_NONE", 0x0},    // This one is not linux related, created by my self.
    {"BTN_MISC", 0x100},
    {"BTN_0", 0x100},
    {"BTN_1", 0x101},
    {"BTN_2", 0x102},
    {"BTN_3", 0x103},
    {"BTN_4", 0x104},
    {"BTN_5", 0x105},
    {"BTN_6", 0x106},
    {"BTN_7", 0x107},
    {"BTN_8", 0x108},
    {"BTN_9", 0x109},
    {"BTN_MOUSE", 0x110},
    {"BTN_LEFT", 0x110},
    {"BTN_RIGHT", 0x111},
    {"BTN_MIDDLE", 0x112},
    {"BTN_SIDE", 0x113},
    {"BTN_EXTRA", 0x114},
    {"BTN_FORWARD", 0x115},
    {"BTN_BACK", 0x116},
    {"BTN_TASK", 0x117},
    {"BTN_JOYSTICK", 0x120},
    {"BTN_TRIGGER", 0x120},
    {"BTN_THUMB", 0x121},
    {"BTN_THUMB2", 0x122},
    {"BTN_TOP", 0x123},
    {"BTN_TOP2", 0x124},
    {"BTN_PINKIE", 0x125},
    {"BTN_BASE", 0x126},
    {"BTN_BASE2", 0x127},
    {"BTN_BASE3", 0x128},
    {"BTN_BASE4", 0x129},
    {"BTN_BASE5", 0x12a},
    {"BTN_BASE6", 0x12b},
    {"BTN_DEAD", 0x12f},
    {"BTN_GAMEPAD", 0x130},
    {"BTN_SOUTH", 0x130},
    {"BTN_A", 0x130},
    {"BTN_EAST", 0x131},
    {"BTN_B", 0x131},
    {"BTN_C", 0x132},
    {"BTN_NORTH", 0x133},
    {"BTN_X", 0x133},
    {"BTN_WEST", 0x134},
    {"BTN_Y", 0x134},
    {"BTN_Z", 0x135},
    {"BTN_TL", 0x136},
    {"BTN_TR", 0x137},
    {"BTN_TL2", 0x138},
    {"BTN_TR2", 0x139},
    {"BTN_SELECT", 0x13a},
    {"BTN_START", 0x13b},
    {"BTN_MODE", 0x13c},
    {"BTN_THUMBL", 0x13d},
    {"BTN_THUMBR", 0x13e},
    {"BTN_DIGI", 0x140},
    {"BTN_TOOL_PEN", 0x140},
    {"BTN_TOOL_RUBBER", 0x141},
    {"BTN_TOOL_BRUSH", 0x142},
    {"BTN_TOOL_PENCIL", 0x143},
    {"BTN_TOOL_AIRBRUSH", 0x144},
    {"BTN_TOOL_FINGER", 0x145},
    {"BTN_TOOL_MOUSE", 0x146},
    {"BTN_TOOL_LENS", 0x147},
    {"BTN_TOOL_QUINTTAP", 0x148},
    {"BTN_STYLUS3", 0x149},
    {"BTN_TOUCH", 0x14a},
    {"BTN_STYLUS", 0x14b},
    {"BTN_STYLUS2", 0x14c},
    {"BTN_TOOL_DOUBLETAP", 0x14d},
    {"BTN_TOOL_TRIPLETAP", 0x14e},
    {"BTN_TOOL_QUADTAP", 0x14f},
    {"BTN_WHEEL", 0x150},
    {"BTN_GEAR_DOWN", 0x150},
    {"BTN_GEAR_UP", 0x151},
    {"BTN_DPAD_UP", 0x220},
    {"BTN_DPAD_DOWN", 0x221},
    {"BTN_DPAD_LEFT", 0x222},
    {"BTN_DPAD_RIGHT", 0x223},
};
} // namespace
#endif /* B40DF103_2A4C_4229_A006_8225B2F06003 */
