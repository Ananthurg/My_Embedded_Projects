# MPLAB IDE generated this makefile for use with GNU make.
# Project: SENSORS_CODE.mcp
# Date: Sun Apr 06 20:10:45 2025

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

SENSORS_CODE.cof : SENSORS_CODE.o
	$(LD) /p18F45K22 /l"C:\Program Files (x86)\Microchip\mplabc18\v3.47\lib" "SENSORS_CODE.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /z__MPLAB_DEBUGGER_PK3=1 /z__ICD2RAM=1 /o"SENSORS_CODE.cof" /M"SENSORS_CODE.map" /W

SENSORS_CODE.o : SENSORS_CODE.c ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdio.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdlib.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/usart.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h SENSORS_CODE.c ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45k22.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdarg.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/pconfig.h ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h
	$(CC) -p=18F45K22 "SENSORS_CODE.c" -fo="SENSORS_CODE.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "SENSORS_CODE.o" "SENSORS_CODE.cof" "SENSORS_CODE.hex"

