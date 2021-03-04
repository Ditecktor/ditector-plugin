#!/bin/sh

# Copy the binary into the obs folder
cp build/obs-plugin.so /lib/obs-plugins/

# Create the configuration folder if necessary 
mkdir -p /usr/share/obs/obs-plugins/ditector
cp -r data/* /usr/share/obs/obs-plugins/ditector
