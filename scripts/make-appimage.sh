#!/bin/bash
# Script to automatize AppImage creation.
# By José Puga 2023. Under MIT License.

# For me, app_name = exec_name
app_name=qmice-remap
tmp_dir=../AppImage
exec_path=../../build-qmice-remap-Desktop-MinSizeRel2
assets_dir=../linuxdeploy-files
profiles_dir=../profiles

cd "$(dirname "$0")" || exit 1
echo -n Deleting $tmp_dir ... 
rm -fr -I "$tmp_dir" 
mkdir -p $tmp_dir/data/profiles
# Profiles files...
cp $profiles_dir/*.profile $tmp_dir/data/profiles
# Config file...
cp $assets_dir/qmice-remap.ini $tmp_dir/data
# linuxdeploy will generate an AppRun.wrapper with this file
cp $assets_dir/AppRun $tmp_dir/AppRun && \
linuxdeploy-x86_64.AppImage  \
    --appdir $tmp_dir \
    --icon-file=$assets_dir/$app_name.png \
    --desktop-file=$assets_dir/$app_name.desktop \
    --executable=$exec_path/$app_name \
    --output appimage --plugin qt && \
mv ./qmice-remap-x86_64.AppImage ../
