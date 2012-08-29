#!/bin/bash
#1MB
date >> EXP_RESULTS
echo '=&= QS algorithm =&=' >> EXP_RESULTS
echo '=&= 1 MB_Eng_Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./nclQSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cpuQSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./gpuQSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
#~ { /usr/bin/time -f "%C,\t%E real time" sh ./cgQSexp1mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS


#10MB
date >> EXP_RESULTS
echo '=&= QS algorithm =&=' >> EXP_RESULTS
echo '=&= 10 MB_Eng_Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./nclQSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cpuQSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./gpuQSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cgQSexp10mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS


#100MB
date >> EXP_RESULTS
echo '=&= QS algorithm =&=' >> EXP_RESULTS
echo '=&= 100 MB_Eng_Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./nclQSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cpuQSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./gpuQSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./cgQSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS
