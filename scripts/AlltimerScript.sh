#!/bin/bash

#~ #Print info of hardware
./clPrintInfo >> EXP_RESULTS

#Single-pattern matching
#Horspool DNA
sh ./DNA_timerScriptHS.sh;
#QS DNA	
sh ./DNA_timerScriptQS.sh;
#QS ENGLISH	
sh ./ENG_QStimerScript.sh;
#Horspool ENGLISH
sh ./ENG_HStimerScript.sh;


#Multi-pattern matching
#Set Horspool DNA
sh ./DNA_SHtimerScript.sh;
#Set Sunday DNA	
sh ./DNA_SQtimerScript.sh;
#Set Sunday ENGLISH	
sh ./ENG_SQtimerScript.sh;
#Set Horspool ENGLISH
sh ./ENG_SHtimerScript.sh;

