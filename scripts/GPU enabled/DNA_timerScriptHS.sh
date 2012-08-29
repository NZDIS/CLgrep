#!/bin/bash
#DNA
#HS
#100MB
date >> EXP_RESULTS
echo '=&= Horspool algorithm =&=' >> EXP_RESULTS
echo '=&= needles length = 1-10 =&=' >> EXP_RESULTS
echo '=&= 100 MB DNA Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./DNAnclHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./DNAcpuHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./DNAgpuHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./DNAcgHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS

date >> EXP_RESULTS
echo '=&= Horspool algorithm =&=' >> EXP_RESULTS
echo '=&= needles length = 1-4 =&=' >> EXP_RESULTS
echo '=&= 100 MB DNA Haystack =&=' >> EXP_RESULTS

echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU non-CL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./ShortNeedleLengthDNAnclHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CPU OpenCL	Local size = 64		Global size = 64*4'  >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./ShortNeedleLengthDNAcpuHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'GPU OpenCL	Local size = 256		Global size = 256*16*5' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./ShortNeedleLengthDNAgpuHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo 'CG  OpenCL' >> EXP_RESULTS
{ /usr/bin/time -f "%C,\t%E real time" sh ./ShortNeedleLengthDNAcgHSexp100mb.sh; } 2>>EXP_RESULTS
echo '*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*' >> EXP_RESULTS
echo '\n' >> EXP_RESULTS
echo done!
cat EXP_RESULTS
