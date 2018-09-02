#!/bin/sh
####################################################################
# author: Hannaneh Barahouei Pasandi
# This is a script to run my simulator program and my RL agent to find the best protocol.                                                 
# It:                                                              
#
#
####################################################################

# ------------------------ General script information
#binary=simulator 
 
#op1=logfile.txt    # "permanent" data files
#op2=throughput.txt


#op5=a                    # temporary data files
#op6=b
#op7=c
#op8=d

#D=0      # are we debugging?

#---------------- Reading state file --------------------------------

#INPUT=states.txt
#OLDIFS=$IFS
#IFS=,
#[ ! -f $INPUT ] && { echo "$INPUT file not found"; exit 99; }

#while read ack backoff prob cw csma protocol
#do
 # echo "ack : $ack"
 # echo "backoff : $backoff"
 # echo "prob : $prob"
 # echo "cw : $cw"
 # echo "csma : $csma"
 # echo "protocol : $protocol"
#done < $INPUT
#IFS=$OLDIFS
#echo "Test" >> input.txt
#while IFS= read -r line;
#do
#    echo "$line" > input.txt
#done <$INPUT
#echo "completed"

# --------------------------------- Compile new binary -------------------
i=0
while [ "$i" -le 59 ]; do
	echo sim: Compiling...
    rm -f $binary
	make

	# ------------------------------ If it compiled, run it ---------------
	if [ -f $binary ]
  	then
     	echo sim: Successful compilation...
     	./simulator "$i"
     	 i=$(($i+1))
    fi
done


#---------------------------------- Read the reward ---------------------




