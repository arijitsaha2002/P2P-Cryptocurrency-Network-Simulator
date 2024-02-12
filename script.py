from multiprocessing import Pool
import os

f = open('cmds_to_run', 'r')

lst = []
for line in f.readlines():
    lst.append(line.strip())
p = Pool()
p.map(os.system, lst)

f.close()
