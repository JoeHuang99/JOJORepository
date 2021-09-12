
from datetime import datetime
import threading
import time

dataTemp = []
isDivisible = False
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
    file.write("Output Time:" + str(time.ctime()))

def mergeCall(data):
    if len(data) == 1:
        return
    global dataTemp
    dataTemp = []
    threads = []
    counter = 0
    while counter < len(data)-1:
        left = data[counter]
        right = data[counter+1]
        t = threading.Thread(target=merge(left, right, dataTemp))
        t.start()
        threads.append(t)
        counter = counter + 2

    for thread in threads:
        thread.join()
    if len(data)%2 == 1:
        dataTemp.append(data[len(data)-1])
    data = dataTemp
    mergeCall(data)


def merge(left, right, dataTemp):
    temp = [0 for n in range(len(left)+len(right))]
    i = 0
    j = 0
    k = 0

    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            temp[k] = left[i]
            i = i + 1
        else:
            temp[k] = right[j]
            j = j + 1
        k = k + 1

    while i < len(left):
        temp[k] = left[i]
        i = i + 1
        k = k + 1

    while j < len(right):
        temp[k] = right[j]
        j = j + 1
        k = k + 1

    dataTemp.append(temp)

def devide(data, k): # 如果整除會分成K段，如果沒有整除會分成K+1段
    smallData = []
    for i in range(0, len(data), len(data)//k):
        smallData.append(data[i:i+(len(data)//k)])
    return smallData

def dynamicThreadAndSorting(k, data):
    # Start all threads.
    threads = []

    for n in range(0, k):
        t = threading.Thread(target=bubbleSort(data[n]))
        t.start()
        threads.append(t)

    if isDivisible == False: # 額外做一次排序
        t = threading.Thread(target=bubbleSort(data[len(data)-1]))
        t.start()
        threads.append(t)
    #print(isDivisible)

    for t in threads:
        t.join()
    #print(data[len(data)-1])

def writeFile( data, file, cpuTime ):
    file.write("Sort:" + "\n")
    for i in data:
        for j in i:
            file.write(str(j) + "\n")
    file.write("CPU Time:" + str(cpuTime) + "\n")
    file.write("Output Time:" + str(datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-1]) + "+08:00")
def main():
#if __name__ == '__main__':
    '''
    print("請輸入檔案名稱：")
    fileName = input() + ".txt"
    print("請輸入要切成幾份：")
    k = input()
    inputFile = open(fileName, "r")
    '''
    global isDivisible
    print("請輸入檔案名稱(不包含.txt)：")
    fileName = input()
    print("請輸入要切成幾份：")
    k = int(input())
    inputFile = open(fileName+".txt","r") # 開啟檔案
    data = readFile(inputFile) # 讀取檔案
    if len(data)%k == 0:
        isDivisible = True
    inputFile.close() # 關閉檔案
    data = devide(data, k) # 分割資料
    #print(data[len(data)-1])
    start = time.time() # 開始計時
    dynamicThreadAndSorting(k, data) # 進行排序
    #print(data[len(data) - 1])
    mergeCall(data)
    data = dataTemp
    #print(data)
    end = time.time()  # 結束計時
    print(end - start)
    '''
    start = time.time()
    bubbleSort(data)
    end = time.time()
    '''
    #print(end-start)
    outputFile = open(fileName+"_output2.txt","w")
    writeFile(data, outputFile, end-start)




