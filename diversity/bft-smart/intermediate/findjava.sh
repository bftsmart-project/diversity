#!/bin/bash
echo $(dirname $(dirname $(readlink -f $(which javac))))
