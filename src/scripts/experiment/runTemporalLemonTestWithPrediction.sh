#!/bin/bash

thisDir="$(dirname $0)"
thisDir="$(readlink -f "$thisDir")"

lemonProgramDir="$thisDir"/../../bin/main/centrality/tests
log4cppDep="$thisDir"/../../dep/log4cpp/lib/
date=$(date +%Y-%m-%d-%H-%M-%S)
info_tmp_dir=info_tmp_"$date"

experimentType=checkall

if [ "$#" -lt 3 ]; then
  echo "Usage: runTemporalLemonTestWithPrediction.sh <input> <outputFolder> <prediction_folder>"
else
  # if enabled then parameters of the test are logged 
  if [ "$experimentType" == "checkall" ]; then
    source "$thisDir"/create_temporal_experiment_info_with_predictions
  fi

  #delete output directory if exists
  rm -r "$2"

  # run lemon program
  pushd "$lemonProgramDir"
  export LD_LIBRARY_PATH="$log4cppDep"
  ./temporal_centrality_for_predictions_test "$@"
  popd

  if [ "$#" -gt 3 ]; then
    pushd "$thisDir"
    # generate html table from json
    #python ./../visualization/topK_json_to_html.py "$2"/intervals.json "$2"/interval_tables.html
    echo "Refactor is needed for topK_json_to_html.py to handle predicted jsons, where there is no 0. interval for centralities!!!"
    popd
  fi

  if [ "$experimentType" == "checkall" ]; then
    mv "$info_tmp_dir"/info "$2"
    rm -r "$info_tmp_dir"
  fi
fi

