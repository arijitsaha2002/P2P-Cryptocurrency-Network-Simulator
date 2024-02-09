#!/bin/python3
import argparse
import os


parse = argparse.ArgumentParser()
# parse.add_argument("-f", type=str, required=True)
parse.add_argument("-t", type=float, default=7.5)
parse.add_argument("-k", type=float, default=.5)
parse.add_argument("-b", type=int, default=200)
parse.add_argument("-a", type=int, default=50)
parse.add_argument("-z", type=float, default=0.4)
parse.add_argument("-o", type=float, default=0.5)
parse.add_argument("-s", type=int, default=42)
parse.add_argument("-x", type=int, default=10000)
parse.add_argument("-n",type=int,default=100)
args = parse.parse_args()

os.system(f"python3 graph.py {args.n} > graph")
os.system(f"make")

os.system(f"./blockSim {args.a}  {args.b}  graph {args.k} {args.o} {args.s} {args.t} {args.x} {args.z} {f'_{args.n}_{args.k}_{args.o}_{args.t}_{args.z}.log.csv'}")
# os.system(f"echo {args.a}  {args.b}  {args.f} {args.k} {args.o} {args.s} {args.t} {args.x} {args.z}")

