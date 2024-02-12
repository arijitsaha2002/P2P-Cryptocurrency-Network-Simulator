#!/bin/python3
import argparse
import os



def make_blockchain_tree(filename):
    pass


if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("--blockchain_tree", type=str, default=None)
    args = parse.parse_args()
    if args.blockchain_tree:
        make_blockchain_tree(args.blockchain_tree)

