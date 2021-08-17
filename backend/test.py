# import numpy as np
#
# def Arnold(Wimage,a,b,time):
#     M,m=Wimage.shape
#     AN = np.zeros([M,M])
#     for i in range(time):
#         for y in range(M):
#             for x in range(m):
#                 xx=(x+b*y)%M
#                 yy=((a*x)+(a*b+1)*y)%M
#                 AN[yy][xx]=Wimage[y][x]
#         Wimage=AN.copy()
#     return Wimage
#
#
# def DeArnold(Wimage, a, b, time):
#     M, m = Wimage.shape
#     AN = np.zeros([M, M])
#     for i in range(time):
#         for y in range(M):
#             for x in range(m):
#                 xx = ((a * b + 1) * x - b * y) % M
#                 yy = (-a * x + y) % M
#                 AN[yy][xx] = Wimage[y][x]
#         Wimage = AN.copy()
#     return Wimage
#
# a = np.array([1,2,3,4,5,6,7,8,9])
from datetime import datetime
import time

a = "1ab21d8355cfa17f8e61194831e81a8f22bec8c728fefb747ed035eb5082aa2b"
print(len(bin(int(a,16))))
print(bin(int(a,16)))
print(len(a))
print(datetime.now())
print(datetime.fromtimestamp(str(time.time())))