#!/bin/bash

date_title=`date +%Ex\(%a\)`
date_fname=source/dialy/`date +%Y%m%d`.md
temp="temp-dialy.md"

sed "s/# title/# ${date_title}/g" $temp | cat > $date_fname
