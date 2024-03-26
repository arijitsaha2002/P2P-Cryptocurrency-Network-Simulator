#!/bin/bash
nPeers=100
echo -n > cmds_to_run
mkdir -p logs/
python3 graph.py $nPeers > logs/graph_$nPeers

for g1 in {0.3,0.4,0.5}; 
do
	for g2 in {0.0,0.3};
	do
		echo ./blockSim \
			--interarrival_transaction_time 0.1 \
			--interarrival_block_time 10 \
			--max_blocks 100 \
			--initial_amt 50 \
			--frac_slow 0.5 \
			--seed 42 \
			--max_transactions -1 \
			--graph "logs/graph_$nPeers" \
			--g1 "$g1" \
			--g2 "$g2" >> cmds_to_run;
	done;
done;
python3 script.py
rm cmds_to_run
