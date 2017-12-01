# bftsmart-diversity
bftsmart-diversity is a library for creating applications in BFT-SMaRt with diversity.

## Cloning

This project must be cloned with the `--recursive` option, in order to also clone sub-repositories containing dependencies.

    git clone --recursive https://github.com/caioycosta/bftsmart-diversity.git


## Requirements
For Ubuntu distributions, run the script

    ./install_prereqs.sh

to install all required packages. If not on a Ubuntu distributions, you need the following packages or equivalents:

* build-essential 
* autoconf 
* libtool 
* make 
* openjdk-8-jdk 
* python-setuptools 
* pkg-config 
* golang

## Building
1. Run make
2. That's all!

**Note:** the project was set-up to be built with Go version **<= 1.8**. If you'd like to use more recent Go releases, please refer to the comments in files `demos/go/<name-of-demo>/Makefile` (i.e. the Makefile in all of the Go demos) in order to adjust the build for it.

## Running
Demos are in the demos/ folder. Do the following in order to run the helper script which guides you through running a demo.

    cd demos
    ./run_demos.sh


