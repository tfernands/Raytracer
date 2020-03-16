@echo off
setlocal enableextensions enabledelayedexpansion

:: 1:name 2:width 3:height 4:spp 5:xs 6:ys 7:xf 8:yf 9:seed

set PATH=%1
set NAME=%2
set /a W=%3
set /a H=%4
set /a SPP=%5
set /a SEED=0
set /a CORE=8
set /a BSIZE=100
set /a BLOCKSX = %W%/%BSIZE%
set /a BLOCKSY = %H%/%BSIZE%

set /a tempvar = %BLOCKSX%*%BSIZE%
if %tempvar% lss %W% (set /a BLOCKSX=%BLOCKSX%+1)
set /a tempvar = %BLOCKSY%*%BSIZE%
if %tempvar% lss %H% (set /a BLOCKSY=%BLOCKSY%+1)
set /a BLOCKSX = %BLOCKSX%-1
set /a BLOCKSY = %BLOCKSY%-1

echo PATH=%PATH% NAME=%NAME% W=%W% H=%H%, SPP=%SPP%, SEED=%SEED%, BLOCKSX=%BLOCKSX%, BLOCKSY=%BLOCKSY%

for /l %%i in (0, 1, %BLOCKSY%) do (
	for /l %%j in (0, 1, %BLOCKSX%) do (
		set /a xs = %%j*%BSIZE%
		set /a xf = %%j*%BSIZE% + 1*%BSIZE%
		set /a ys = %%i*%BSIZE%
		set /a yf = %%i*%BSIZE% + 1*%BSIZE%
		if !xf! gtr %W% (set /a xf = %W%)
		if !yf! gtr %H% (set /a yf = %H%)
		echo %PATH% %NAME% %W% %H% %SSP% %SEED% !xs! !xf! !ys! !yf!
		rem call %PATH% %NAME% %W% %H% %SSP% %SEED% !xs! !xf! !ys! !yf!
	)	
)