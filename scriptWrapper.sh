b=500
a=50
s=69
echo -n > cmds_to_run
for nPeers in {100,500}
do
        python3 graph.py $nPeers > graph_$nPeers
        for k in {1,10,50,100}
        do
                for o in {0.2,0.4}
                do
                        for t in {0.2,0.5,1.0}
                        do
                                for z in {0.2,0.4}
                                do
                                        echo python3 blockSimWrapper.py -a $a -b $b -k $k -o $o -s $s -t $t -z $z -n $nPeers >> cmds_to_run
                                done
                        done
                done
        done
done

python3 script.py
