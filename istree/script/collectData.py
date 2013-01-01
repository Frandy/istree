#/usr/bin/env python

'''
File: collectData.py
Author: chjd
Date: 2012-12-29
'''

'''
collect data from log1 & log2,
include:
    test index,
    graph info,
        vertex num, edge num, path num
    node count,
        shared during build, & after build
        zero suppressed during zs, and after zero zuppress
        shared during reduce, and after reduce
    time count,
        build,
        zero suppress,
        reduce,
        total
save these info to excel
plot figures
'''

import sys
import os
import re
#import time


pt_graph = re.compile("target vnum:(\d+)\s+enum:(\d+)")

pt_tmp = [r"shared node count: (\d+)",
          r"total node count: (\d+)",
          r"\s+build:\s+(\S+)",
          r"zero suppressed node count: (\d+)",
          r"total node count: (\d+)",
          r"\s+zsuppress:\s+(\S+)",
          r"reduce node count: (\d+)",
          r"total node count: (\d+)",
          r"\s+reduce:\s+(\S+)",
          r"\s+total time:\s+(\S+)",
          r"total path count: (\d+)"]

pt = [re.compile(s) for s in pt_tmp]


def ReadGraphInfo(fp):
    graph = []
    line = fp.readline()
    m = re.match(pt_graph, line)
    if m:
        graph.append(m.group(1))
        graph.append(m.group(2))
    return graph


def ReadLogInfo(fp):
    '''
    read log info, return a list,
    share building node, after build node, build time,
    zs node, after zs node, zs time,
    sharing reduce node, after reduce node, reduce time,
    total time,
    path count
    '''
    cnt = 0
    N = 11
    data = [0 for i in xrange(N)]
    while cnt < N:
        pt_pt = pt[cnt]
        line = fp.readline()
        if not line:
            # end of file
            break
        m = re.match(pt_pt, line)
        if m:
            data[cnt] = m.group(1)
            cnt = cnt + 1
    return [cnt == N, data]


def ReadLog2(cnt):
    '''
    log with graph info
    '''
    fname = "".join(["match/log2_", str(cnt), ".txt"])
    if not os.path.exists(fname):
        return [False, []]
    fp = open(fname)
    graph = ReadGraphInfo(fp)
    if not graph:
        print "graph info error, for log2", cnt
        return [False, []]
    ok, data = ReadLogInfo(fp)
    fp.close()
    if not ok:
        print "node & time info error, for log2", cnt
        return [False, []]
    return [True, graph, data]


def ReadLog1(cnt):
    '''
    log, result of test_normal
    '''
    fname = "".join(["match/log1_", str(cnt), ".txt"])
    if not os.path.exists(fname):
        return [False, []]
    fp = open(fname)
    ok, data = ReadLogInfo(fp)
    fp.close()
    if not ok:
        print "node & time info error, for log1", cnt
        return [False, []]
    return [True, data]


def ReadLog(cnt):
    '''
    process once
    '''
    [ok, graph, data_2] = ReadLog2(cnt)
    if not ok:
        print "skip log2 for cnt", cnt
        return [False, []]
    [ok, data_1] = ReadLog1(cnt)
    if not ok:
        print "skip log1 for cnt", cnt
        return [False, []]
    data = graph + data_1 + data_2
    return [True, data]


def ReadAll(N):
    '''
    read all the logs
    '''
    resname = "res.txt"
    fp = open(resname, "wt")
    for cnt in xrange(N):
        ok, data = ReadLog(cnt)
        if not ok:
            continue
        fp.write(str(cnt) + "\t")
        fp.write("\t".join([str(x) for x in data]) + "\n")
    fp.close()
    print "done for all.", N


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "please set the total num"
        sys.exit()
    N = int(sys.argv[1])
    ReadAll(N)