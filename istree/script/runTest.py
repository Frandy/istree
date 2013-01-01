#/usr/bin/env python

'''
to test the new version stree
first, run case test,
then collect data.
'''

import sys
import os


def RunTest(cnt):
    '''
    run single test,
    first run test_order & test_normal,
    then collect res.
    '''
    print "run test : ", cnt + 1
    cmd_2 = "./test_order > log2.txt"
    cmd_1 = "./test_normal rndg.txt > log1.txt"
    os.system(cmd_2)
    os.system(cmd_1)

    postfix = str(cnt) + ".txt"
    mv_0 = "mv rndg.txt ./compare_res/rndg_" + postfix
    mv_2 = "mv log2.txt ./compare_res/match/log2_" + postfix
    mv_1 = "mv log1.txt ./compare_res/match/log1_" + postfix
    os.system(mv_0)
    os.system(mv_1)
    os.system(mv_2)


def TestAll(N):
    '''
    '''
    cnt = 0
    while cnt < N:
        RunTest(cnt)
        cnt = cnt + 1
    print "test for %d cases done." % (N)


if __name__ == "__main__":
    N = 1
    if len(sys.argv) == 2:
        N = int(sys.argv[1])
    TestAll(N)