import os
import sys
import time
import math

defaultIn = sys.stdin
defaultOut = sys.stdout
path = "application/common/secgear_Enclave1"


def initServer(D, N):
    """
    Initialize the server (generates Ku, Kw) and input D and N. Return CD, Ku, Kw.
    """
    os.system("{0} initServer {1} {2} {3} > buf.tmp".format(path, D, N, math.floor(time.time())))
    sys.stdin = open("buf.tmp", "r")
    CD, Ku, Kw = input(), input(), input()
    os.system("echo > buf.tmp")
    sys.stdin = defaultIn
    return (CD, Ku, Kw)


def passwordEncryption(CPW, CD, N, Ku):
    """
    Input CPW to get NPW. Also used for login verification.
    """
    os.system("{0} passwordEncryption {1} {2} {3} {4} > buf.tmp".format(path, CPW, CD, N, Ku))
    sys.stdin = open("buf.tmp", "r")
    NPW = input()
    os.system("echo > buf.tmp")
    sys.stdin = defaultIn
    return NPW


def embedding(CWID, NPW, CD, N, Ku, Kw):
    """
    Generate x in (0.2, 0.4), mu in (0.2, 0,3), k in [1,15]. Input CWID, NPW. Return x, mu, k, nwid.
    """
    os.system("{0} embedding {1} {2} {3} {4} {5} {6} {7} > buf.tmp".format(path, CWID, NPW, CD, N, Ku, Kw,
                                                                           math.floor(time.time())))
    sys.stdin = open("buf.tmp", "r")
    x, mu, k, nwid = input(), input(), input(), input()
    os.system("echo > buf.tmp")
    sys.stdin = defaultIn
    return (x, mu, k, nwid)

# if __name__=='__main__':
#     #print(initServer(123,321))
#     print(passwordEncryption("c0264e012738712999fde7a71c43e272a36defd11597c4ab5645275901208b1bf0b200703ae4fd1c801fc4becd61b4d239422783999d48527fa807fe7717d6327b48d8465bd78473168371e3d6db3e67fc1b4d66a110f7454cf50df9fbfee97b","0000000000008086943196968789029216924856034222826528111906739900803639236101218675383437733649948047274478395824290486592462213580902459113784421075015835549166277097469778871069562230418722532418783411634996198522784455040788907811833822318035628899492613c8b8644d4a1d76a9aa3960e4a3bbf600f31c2f4c5b294529a09527d36717358d",hex(121304147954531835438253872840513342397921678601098512054588541518280130751566004749220709117175937364357298886933203713536886706766316125237533279021161728021515456906335208707792680208981585475738249431198042652639984379772758656333795873928918372151374018861786825862513094905136142715975401426712425130741),745454334))
#     #print(embedding(153,878,134,5432,5432,54))
