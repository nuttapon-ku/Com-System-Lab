import ctypes
import time

if __name__=="__main__":
    matMult = ctypes.CDLL('/home/win/Desktop/1.2/insertion.so')
    start = time.time()
    matMult.main()
    end = time.time()
    print("Total time : ")
    print(end-start)