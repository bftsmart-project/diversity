#!/bin/bash
top -b -d 7 &
echo sleeping.
sleep 30
kill %1

