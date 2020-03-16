@echo off

set name=%1
set /a width=%2
set /a height=%3
set /a spp=%4
set /a seed=%5

call make
call main.exe %name%.ppm %width% %height% %spp% %seed%
call activate
call python scripts/ppm2png.py %name%.ppm
call del %name%*.ppm
