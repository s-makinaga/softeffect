@echo off
cls
bcc32 -W -w- -nDebag main.cpp
brc32 resource.rc Debag\main.exe
brc32 EffectDialog.rc Debag\main.exe


