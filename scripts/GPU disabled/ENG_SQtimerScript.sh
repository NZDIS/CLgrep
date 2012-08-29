#!/bin/bash
#1-10
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 1 MB_Eng_Haystack =!=' >> EXP_RESULTS
echo '=!!!= 10 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn10h1MB_cPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 256		GS = 256*5*8' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn10h1MB_gPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn10h1MB_Cg.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn10h1MB_NO.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

#10-10
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 10 MB_Eng_Haystack =!=' >> EXP_RESULTS
echo '=!!!= 10 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn10h10MB_cPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 64		!!!64*5*32!!!' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn10h10MB_gPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn10h10MB_Cg.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn10h10MB_NO.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

#100-10
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 100 MB_Eng_Haystack =!=' >> EXP_RESULTS
echo '=!!!= 10 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn10h100MB_cPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 64		!!!64*5*32!!!' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn10h100MB_gPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn10h100MB_Cg.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn10h100MB_NO.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

#1-20
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 1 MB_Eng_Haystack =!=' >> EXP_RESULTS
echo '=!!!= 20 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn20h1MB_cPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 64		!!!64*5*32!!!) with JUMP' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn20h1MB_gPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn20h1MB_Cg.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn20h1MB_NO.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

#10-20
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 10 MB_Eng_Haystack =!=' >> EXP_RESULTS
echo '=!!!= 20 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn20h10MB_cPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 256		GS = 256*5*8) with JUMP' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn20h10MB_gPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn20h10MB_Cg.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn20h10MB_NO.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

#100-20
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 100 MB_Eng_Haystack =!=' >> EXP_RESULTS
echo '=!!!= 20 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn20h100MB_cPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 256		GS = 256*5*8)' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn20h100MB_gPU.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh SQn20h100MB_Cg.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh SQn20h100MB_NO.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS
