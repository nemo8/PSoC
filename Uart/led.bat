
set pause=1
set COMPORT=COM16
REM @echo off
mode %COMPORT% BAUD=115200 PARITY=n DATA=8

cls

FOR /L %%G IN (1,1,10) DO ( echo %%G

	set /p x="b" <nul >\\.\%COMPORT%
	TIMEOUT /T %pause% 
	set /p x="b" <nul >\\.\%COMPORT%
	TIMEOUT /T %pause%

	set /p x="g" <nul >\\.\%COMPORT%
	TIMEOUT /T %pause%
	set /p x="g" <nul >\\.\%COMPORT%
	TIMEOUT /T %pause%

	set /p x="r" <nul >\\.\%COMPORT%
	TIMEOUT /T %pause%
	set /p x="r" <nul >\\.\%COMPORT%
	TIMEOUT /T %pause%

)