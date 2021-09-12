
import time
from datetime import datetime

def bubbleSort( data ):
    n = len( data )
    for i in range( 0, n - 1, 1 ):
        for j in range( n - 1, i, -1 ):
            if data[j] < data[j-1]:
                data[j], data[j-1] = data[j-1], data[j]

def readFile( file ):
    data = []
    for i in file.readlines():
        data.append(int(i.strip('\n')))
    return data

def writeFile( data, file, cpuTime ):
    file.write("Sort:" + "\n")
    for i in data:
        file.write(str(i) + "\n")
    file.write("CPU Time:" + str(cpuTime) + "\n")
    file.write("Output Time:" + str(datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-1]) + "+08:00")

def main():
#if __name__ == '__main__':
    '''
    print("請輸入檔案名稱：")
    fileName = input() + ".txt"
    inputFile = open(fileName, "r")
    '''
    print("請輸入檔案名稱(不包含.txt)：")
    fileName = input()
    inputFile = open(fileName+".txt","r")
    data = readFile(inputFile)
    start = time.time() # 開始計時
    bubbleSort(data)
    end = time.time() # 結束計時
    print(end-start)
    inputFile.close()
    outputFile = open(fileName+"_output1.txt","w")
    writeFile(data, outputFile, end-start)




