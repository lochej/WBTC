#!/bin/bash

shell=~/intelFPGA_lite/18.1/embedded/embedded_command_shell.sh

#Create a directory to store the output file
sopcfile=./DE0_HPS_Example.sopcinfo
dtsfile=hps_fpga_device_tree.dts
exportdir=./sw/include
dtsdir=./sw/dts
includefile=hps_soc_system.h
mkdir -p $exportdir
mkdir -p $dtsdir

$shell sopc-create-header-files $socpfile --single $exportdir/$includefile --module hps_0

$shell sopc2dts $sopcfile > $dtsdir/$dtsfile

echo "Exported device tree from $sopcfile into $dtsdir/$dtsfile"
