import ctypes

if __name__=="__main__":
    matMult = ctypes.CDLL('/app/insertion.so')
    matMult.main()