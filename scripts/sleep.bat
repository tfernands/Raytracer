@echo off

for /l %%i in (0, 1, 1000) do (
	echo counter id= %1  %%i 
)

exit