#!/bin/python3
import argparse
import os


parse = argparse.ArgumentParser()
# parse.add_argument("-f", type=str, required=True)
parse.add_argument("-t", "--interarrival_transaction_time", type=float, default=1)
parse.add_argument("-k", "--interarrival_block_time", help="mean interarrival block time", type=float, default=20)
parse.add_argument("-b", "--max_blocks", help="number of blocks to terminate", type=int, default=200)
parse.add_argument("-a", "--initial_amt", help="initial coins for each node", type=int, default=50)
parse.add_argument("-z", "--frac_slow", help = "fraction of slow(slow on network,  high latency) nodes", type=float, default=0.2)
parse.add_argument("-o", "--frac_low_cpu", help="fraction of low cpu(low hashing power) nodes", type=float, default=0.2)
parse.add_argument("-s", "--seed",help="seed for randomness in simultaion",type=int, default=42)
parse.add_argument("-x", "--max_transactions",help="Number of Transactions for stopping criteria (default -1, not used as stopping criteria)" ,type=int, default=-1)
parse.add_argument("-n", "--num_peers" , help = "Number of peers in blockhchain network(default 100)", type=int,default=100)
args = parse.parse_args()

os.system(f"python3 graph.py {args.num_peers} > graph")
os.system(f"make")

os.system(f"./blockSim {args.initial_amt}  {args.max_blocks}  graph {args.interarrival_block_time} {args.frac_low_cpu} {args.seed} {args.interarrival_transaction_time} {args.max_transactions} {args.frac_slow} {f'_{args.num_peers}_{args.interarrival_block_time}_{args.frac_low_cpu}_{args.interarrival_transaction_time}_{args.frac_slow}.log.csv'}")
# os.system(f"echo {args.a}  {args.b}  {args.f} {args.k} {args.o} {args.s} {args.t} {args.x} {args.z}")

