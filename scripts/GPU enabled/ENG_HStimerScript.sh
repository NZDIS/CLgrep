#!/bin/bash
#1MB
date >> EXP_RESULTS
echo '=&= Horspool algorithm =&=' >> EXP_RESULTS
echo '=&= 1 MB_Eng_Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./nclHSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cpuHSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./gpuHSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh ./cgHSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS


#~ 10MB
date >> EXP_RESULTS
echo '=&= Horspool algorithm =&=' >> EXP_RESULTS
echo '=&= 10 MB_Eng_Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./nclHSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cpuHSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./gpuHSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cgHSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS


#~ 100MB
date >> EXP_RESULTS
echo '=&= Horspool algorithm =&=' >> EXP_RESULTS
echo '=&= 100 MB_Eng_Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./nclHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cpuHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./gpuHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cgHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS
