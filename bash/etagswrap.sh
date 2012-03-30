#! /bin/bash

if [ $# -lt 2 ]
then
    echo "Please give directory and extension to the command"
    exit 0
fi

root_dir="$1"
file_ext="$2"
tag_file_path=~/.emacs.d/tags/TAGS.auto_generated

echo $root_dir
echo $file_ext

find $root_dir -name "*.$file_ext" -print \
    -exec etags -a -o $tag_file_path {} \;

