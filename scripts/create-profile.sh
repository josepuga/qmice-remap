#!/bin/bash
# Script to create a qmice-remap profile file from a mouse.
# https://github.com/josepuga/qmice-remap
# By José Puga 2023. Under MIT License.

# Does evtest is installed?
#TODO: python3 too
evtest_command=$(whereis evtest | awk '{print $2;}')
if [ -z "$evtest_command" ]; then
    echo "evtest must be installed."
    exit 1
fi


# Check parameter
if [ -z "$1" ]; then
    echo "Usage $0 DeviceNumber"
    exit 1
fi
event_path="/dev/input/event$1"

# If not a Character device...
if [ ! -c "$event_path" ]; then
    echo "Bad Device Number: $1"
    exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
    echo Run with sudo
    exit 1
fi

# Trap temp file for delete it on any exit
tmp_file=$(mktemp)
trap 'rm "$tmp_file"' EXIT

# Run evtest in background and check for errors.
evtest "$event_path"  | tee "$tmp_file" &> /dev/null &
if [ ! "$?" -eq 0 ]; then
    echo Error running evtest

    exit 1
fi
evtest_pid=$!

echo evtest is lissening your mouse. Click all your buttons ONE BY ONE.
for n in {8..1}; do
    echo "$n" seconds left...
    sleep 1
done
kill -9 "$evtest_pid"

# Parse device name
# $1: File to parse
# Return: string with the name
GetName() {
    grep "Input device name" "$1" | \
        awk -F: '{print $2}' | tr -d '"'
}

# Parse buttons
# Every button info is in 2 lines. The first one, has the KEYBOARD_ 
# The second one, the button tag (BTN_XXX)
# Sample output...
#  Event: time 1700309638.527939, type 4 (EV_MSC), code 4 (MSC_SCAN), value 90004
#  Event: time 1700309638.271909, type 1 (EV_KEY), code 273 (BTN_RIGHT), value 1
# $1: File to parse
# Return: One line by button, ie: 90004=BTN_RIGHT
GetButtons() {
    is_keycode=1
    IFS=$'\n'
    lines=$(grep -e ^Event "$1" | grep -v 'SYN_REPORT\|EV_REL')
    key=
    value=
    raw_key_value=
    for line in $lines; do
        if [ "$is_keycode" -eq 1 ]; then
            is_keycode=0
            key=$(echo "$line" | awk '{print $NF}')
        else
            is_keycode=1
            value=$(echo "$line" | awk '{print $(NF-2)}' | tr -d '(),')
            raw_key_value="$raw_key_value$key=$value\n"
        fi
    done
    IFS=$' '    
    echo -e "$raw_key_value" | sort -u

}

# Parse bus, vendor, product and version. Extract from line like this:
# Input device ID: bus 0x3 vendor 0x47d product 0x1020 version 0x111
# $1: File to parse
# Return: string with the 4 numbers
GetInfo () {
    # Extract those numbers and rip off the "0x"
    raw_info=$(grep "Input device ID" "$1" | \
            awk '{print $5, $7, $9, $11}' | \
            sed 's/0x//g' | tr " " "\n")
    # Now we need to pad with 4 len every data with '0'. IE. 47d => 047d
    # I don't know how to do that in Bash with strings, (`printf "%04d" $param` 
    # only works with numbers). Using python instead.
    result=( $(echo $raw_info) )
    for i in "${!result[@]}"; do
        result[i]=$(python3 -c "print('${result[i]}'.rjust(4, '0'))")
    done
    echo "${result[@]}"
}



name=$(GetName "$tmp_file")
info=( $(GetInfo "$tmp_file") )

# Creating the name of the profile file, now we get the info.
profile_file="/tmp/${info[0]}-${info[1]}-${info[2]}-${info[3]}.profile"
cat <<EOF > "$profile_file"
# Mouse profile file for qmice-remap.
#
# Read tutorial on https://github.com/josepuga/qmice-remap/blob/main/doc/tutorial-create-profile-EN.md

[info]
name=$name
uploader=
comment=formula 

[id]
bustype=${info[0]}
vendor=${info[1]}
product=${info[2]}
version=${info[3]}

EOF

buttons=$(GetButtons "$tmp_file" | grep -v ^$) #TODO: Check this missing return
printf "%s\n" "[default buttons]" "$buttons" >> "$profile_file"
cat <<EOF >> "$profile_file"

[gui]
image=
EOF

# The real owner needs the profile
[ "$SUDO_USER" ] && chown "$SUDO_UID:$SUDO_GID" "$profile_file"

buttons_count=$(echo "$buttons" | wc -w)
echo ''
echo "$buttons_count" buttons recorded.
echo Your profile has been saved on "$profile_file"

exit






