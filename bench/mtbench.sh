#!/bin/sh -
# ======================================================================
#  G4Bench benchmark (MT)
# ======================================================================
export LANG=C

# ======================================================================
# run parameters
# ======================================================================
th_list=(1 2 4 8)
#th_list=(1 2 4 8 16 24)
#h_list=(1 2 4 8 16 24 32)
#th_list=(1 2 4 8 16 24 32 40 48)
#th_list=(1 2 4 8 16 24 32 40 48 60 72)
#th_list=(1 2 4 8 16 24 32 40 48 60 72 96)
#h_list=(1 2 4 8 16 24 32 40 48 60 72 96 128)
#th_list=(1 2 4 8 16 24 32 40 48 60 72 96 128 160)

app=$1
uevent=$2
log=$3

sys=`uname`
if [ ${sys} = "Darwin" ]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

# ======================================================================
echo "thread, eps, edep" > $log.dat

for t in "${th_list[@]}"
do
  echo "running... #threads = $t"
  nevent=`expr $uevent \* $t`
  $app -n $t -j $nevent -p "${cpu_info}" -b $log > $log-n$t.log 2>&1
  echo -n "$t,  " >> $log.dat
  sed -n 2p jtest.out >> $log.dat
  mv g4bench.json $log-n$t.json
done
