import os
import sys
import time
import math

defaultIn = sys.stdin
defaultOut = sys.stdout

def initServer(D, N):
    """
    Initialize the server (generates Ku, Kw) and input D and N. Return CD, Ku, Kw.
    """
    os.system("application/common/secgear_Enclave1 initServer {0} {1} {2} > buf.tmp".format(D, N, math.floor(time.time())))
    sys.stdin = open("buf.tmp","r")

    CD, Ku, Kw = input(), input(), input()
    os.system("echo > buf.tmp")
    sys.stdin = defaultIn
    return (CD, Ku, Kw)

def passwordEncryption(CPW, CD, N, Ku):
    """
    Input CPW to get NPW. Also used for login verification.
    """
    os.system("application/common/secgear_Enclave1 passwordEncryption {0} {1} {2} {3} > buf.tmp".format(CPW, CD, N, Ku))
    sys.stdin = open("buf.tmp","r")
    NPW = input()
    os.system("echo > buf.tmp")
    sys.stdin = defaultIn
    return NPW

def embedding(CWID, NPW, CD, N, Ku, Kw):
    """
    Generate x in (0.2, 0.4), mu in (0.2, 0,3), k in [1,15]. Input CWID, NPW. Return x, mu, k, nwid. 
    """
    os.system("application/common/secgear_Enclave1 embedding {0} {1} {2} {3} {4} {5} {6} > buf.tmp".format(CWID, NPW, CD, N, Ku, Kw, math.floor(time.time())))
    sys.stdin = open("buf.tmp","r")
    x, mu, k, nwid = input(), input(), input(), input()
    os.system("echo > buf.tmp")
    sys.stdin = defaultIn
    return (x, mu, k, nwid)

if __name__=='__main__':
    print(initServer(123,321))
    print(passwordEncryption(342,145,6534,734))
    print(embedding(153,878,134,5432,5432,54))
