#!/bin/bash

ls -1 /dev > ~/before.txt; 

read -p "Plug in device and press something to continue";
ls -1 /dev > ~/after.txt;
diff ~/before.txt ~/after.txt;
rm ~/before.txt;
rm ~/after.txt
