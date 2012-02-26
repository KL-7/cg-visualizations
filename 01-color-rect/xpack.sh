#!/bin/bash
cp release/bin/color-rect.exe release/bin/color-rect.exe.bak 
upx --best release/bin/color-rect.exe
upx --t release/bin/color-rect.exe
