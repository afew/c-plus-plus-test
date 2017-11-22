@echo off
set /a JOB_NUM=%NUMBER_OF_PROCESSORS%*4/3

echo compile ...
echo.

del /F /Q /S obj
rmdir /Q /S obj

del /F /Q /S libs
rmdir /Q /S libs

call ndk-build -j %JOB_NUM% -B

echo.
echo *****************************************
echo ** library copy ...
echo *****************************************
echo.

call xcopy /A/C/E/Q/R/Y obj\local\armeabi\*.a ..\epp_aos_lib\
