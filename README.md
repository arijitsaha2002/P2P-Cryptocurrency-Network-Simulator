# P2P-Cryptocurrency-Network-Simulator

CS765 Assignment  to build a discrete event simulator for Peer-to-Peer Cryptocurrency Network

## Running the Simulator

To run the simulator with default arguments, use blockSimWrapper.py
The following is the help of the same to change the parameters.

```bash
$ ./blockSimWrapper.py
usage: blockSimWrapper.py [-h] [-t INTERARRIVAL_TRANSACTION_TIME]
                          [-k INTERARRIVAL_BLOCK_TIME] [-b MAX_BLOCKS] [-a INITIAL_AMT]
                          [-z FRAC_SLOW] [-o FRAC_LOW_CPU] [-s SEED] [-x MAX_TRANSACTIONS]
                          [-n NUM_PEERS]

options:
  -h, --help            show this help message and exit
  -t INTERARRIVAL_TRANSACTION_TIME, --interarrival_transaction_time INTERARRIVAL_TRANSACTION_TIME
  -k INTERARRIVAL_BLOCK_TIME, --interarrival_block_time INTERARRIVAL_BLOCK_TIME
                        mean interarrival block time
  -b MAX_BLOCKS, --max_blocks MAX_BLOCKS
                        number of blocks to terminate
  -a INITIAL_AMT, --initial_amt INITIAL_AMT
                        initial coins for each node
  -z FRAC_SLOW, --frac_slow FRAC_SLOW
                        fraction of slow(slow on network, high latency) nodes
  -o FRAC_LOW_CPU, --frac_low_cpu FRAC_LOW_CPU
                        fraction of low cpu(low hashing power) nodes
  -s SEED, --seed SEED  seed for randomness in simultaion
  -x MAX_TRANSACTIONS, --max_transactions MAX_TRANSACTIONS
                        Number of Transactions for stopping criteria (default -1, not used as
                        stopping criteria)
  -n NUM_PEERS, --num_peers NUM_PEERS
                        Number of peers in blockhchain network(default 100)

```

### Running in docker environment

To run the simulator in docker environment, build the image and run the containers using the following commands
```bash
docker build . -t <image name>
docker run -ti -v $(pwd):/code <image name>
```

## Results

The results of the simulator are logged in `.log.csv` files in the current directory.

### Log format

File names of logs are appended with params resulting in filenames of format
```
<file_name>_<args.num_peers>_<args.interarrival_block_time>_<args.frac_low_cpu>_<args.interarrival_transaction_time>_<args.frac_slow>.log.csv
```

#### `block_info_<params>.log.csv`

- `id`

    Block ID of the block
- `pid`

    Block ID of parent block
- `create_time`
    
    Time at which the block was created
- `creator`

    Node ID that created the block

The first row corresponds to the genesis block, hence the time is nan and creator and pid are kept -1.


#### `node_info_<params>.log.csv`

- `id`

    Node id
- `is_fast`

    **1**, if node has low network latency
- `is_high_cpu`
    
    **1**, if node is having high hashing power which is 10 times of hashing power of low hashing power nodes

#### `block_recieve_node<nodeID>_<params>.log.csv`

This file has the information of time when blocks are recieved by node corresponding to `nodeID`

- `id`
    
    BlockID of recieved block
- `timestamp`
    
    Time of arrival of block at node `nodeID`

## Analysis - Visual Representation of logs

For a complete visual representation, use the file `analysis.py`.

```bash
usage: analysis.py [-h] [--blockchain_tree] [--node_info NODE_INFO]
                   [--blockchain_info BLOCKCHAIN_INFO] [--full_analysis FULL_ANALYSIS]

options:
  -h, --help            show this help message and exit
  --blockchain_tree
  --node_info NODE_INFO
  --blockchain_info BLOCKCHAIN_INFO
  --full_analysis FULL_ANALYSIS
```
For using the `full_analysis` option, put current directory containing the `block_info_<params>.log.csv` and `node_info_<params>.log.csv` as arguments. This option can also be used to compare results for different parameters.

`blockchain_tree` option can be used to plot the tree of at most 200 blocks as a graph.

## Sample Results

`sample_logs` directory has the sample results of simulator on various parameter combinations.

The following image shows the comparison of the contribution of blocks in longest chain of different types of nodes.
![](sample_logs/longest_chain_contrib_comp.png)

The following images are sample blockchain trees with high and low forking.
![](sample_logs/tree_100_20.0_0.2_0.2_0.2.png)
![](sample_logs/tree_100_1.0_0.2_0.2_0.2.png)
