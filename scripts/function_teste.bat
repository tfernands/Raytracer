@echo off
setlocal enableextensions enabledelayedexpansion

:: carriage return character set in variable cr
FOR /f %%a IN ('copy "%~f0" nul /z') DO set "cr=%%a"

FOR /l %%b IN (0 1 10) DO (
	start :SAY_HELLO
)

exit /b

:SAY_HELLO
	FOR /L %%I in (0, 1, 4) DO (
		echo "%%I sHello World"
	)
	
	exit /b 0
