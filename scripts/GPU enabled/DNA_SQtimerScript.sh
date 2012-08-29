#!/bin/bash

#100-10
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 100 MB DNA Haystack =!=' >> EXP_RESULTS
echo '=!!!= 10 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn10h100MB_cPUSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 64		!!!64*5*32!!!' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn10h100MB_gPUSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn10h100MB_CgSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn10h100MB_NOSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

#100-20
date >> EXP_RESULTS
echo '=!= Set SUNDAY algorithm =!=' >> EXP_RESULTS
echo '=!= 100 MB DNA Haystack =!=' >> EXP_RESULTS
echo '=!!!= 20 Needles =!!!=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64	Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn20h100MB_cPUSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local Mem	LS = 256		GS = 256*5*8)' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn20h100MB_gPUSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn20h100MB_CgSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'Non-OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh DNAn20h100MB_NOSetQuickSearch.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS
