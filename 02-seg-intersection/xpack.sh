#!/bin/bash
cp release/bin/seg-intersect.exe release/bin/seg-intersect.exe.bak
upx --best release/bin/seg-intersect.exe
upx --t release/bin/seg-intersect.exe
