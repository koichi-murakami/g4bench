#!/bin/sh -
# ======================================================================
#  G4Bench score
# ======================================================================
export LANG=C

# ======================================================================
# run parameters
# ======================================================================
export G4DATA=/opt/geant4/data

# ======================================================================
# help message
# ======================================================================
show_help() {
cat <<EOF

\`bench_score.sh' run benchmark score.

Usage: bench_score.sh [OPTION]

Options:
  -h, --help                display this help and exit
  -s, --single              run sigle thread score
  -m, --multi               run multi-thread score
EOF
}

# ======================================================================
# functions
# ======================================================================
check_error() {
  if [ $? -ne 0 ]; then
    exit -1
  fi
}

show_line() {
echo "========================================================================"
}

# ======================================================================
# main
# ======================================================================
single_mode=0
mt_mode=0

# parsing options
while test $# -gt 0
do
  case $1 in
    -*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
    *) optarg= ;;
  esac

  case $1 in
    --help|-h) show_help;  exit 0 ;;
    --single|-s)    single_mode=1 ;;
    --multi|-m)     mt_mode=1 ;;
    # ---------------------------------------------------------------
  esac
  shift
done

#
sys=`uname`
if [ ${sys} = "Darwin" ]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

#
if [ -d run ]; then
  echo "[ERROR] run directory already exists."
  exit -1
fi
mkdir run
cd run

show_line
echo " Execute G4Bench score"
show_line

# ======================================================================
cat << EOD > g4bench.conf
{
  Run : {
    Seed : 123456789,
    G4DATA : "${G4DATA}"
  },
  Primary : {
    particle  : "e-",
    energy    : 1000.0,   // MeV
    position  : [ 0., 0., -45. ],  // cm
    direction : [ 0., 0., 1.],
  }
}
EOD

#
NPROCS=`nproc`
NEVENTS=20000
NEVENTS_PAR=`expr $NEVENTS \* $NPROCS`
LOG=ecal_e1000.log

# single thread
if [ $single_mode = "1" ]; then
  ../../ecal/ecal -j -b ecal_e1000 -p "${cpu_info}" ${NEVENTS} > ${LOG} 2>&1
fi

# multi-thread thread
if [ $mt_mode = "1" ]; then
  ../../ecal/ecal-mt -j -b ecal_e1000 -p "${cpu_info}" -n ${NPROCS} ${NEVENTS_PAR} > ${LOG} 2>&1
fi

if [ -e g4bench.json ]; then
  eps=`cat g4bench.json | jq ".eps"`
  score=`echo $eps \* 1000 | bc`
fi

echo "===================================================="
echo "** CPU : $cpu_info"

if [ $single_mode = "1" ]; then
  echo "** Single Score = $score"
fi

if [ $mt_mode = "1" ]; then
  echo "** NPROC = $NPROCS"
  echo "** MT Score = $score"
fi

# ======================================================================
cd ..
exit $?
