#! /bin/bash

if [ $# -lt 2 ]
then
    echo "Please give directory and extension to the command"
    exit 1
fi

root_dir="$1"
file_ext="$2"
tag_file_path=~/.emacs.d/tags/TAGS.auto_generated

if [ ! -f "$tag_file_path" ]
then
    tag_file_dir=$(dirname $tag_file_path)
    if [ ! -e "$tag_file_dir" ]
    then
        mkdir -p $tag_file_dir
    fi
    touch $tag_file_path
fi

find $root_dir -name "*.$file_ext" -print \
    -exec etags -a -o $tag_file_path {} \;

