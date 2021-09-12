
from datetime import datetime
import multiprocessing
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
    processes = []
    counter = 0

    while counter < len(data)-1:
        left = data[counter]
        right = data[counter+1]

        p = multiprocessing.Process(target=merge(left, right, dataTemp))
        p.start()

        processes.append(p)
        counter = counter + 2
    #start = time.time()
    for process in processes:
        #start = time.time()
        process.join()
        #end = time.time()
        #print(end - start)
    #end = time.time()
    #print(end-start)
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

def devide(data, k):
    smallData = []
    for i in range(0, len(data), len(data)//k):
        smallData.append(data[i:i+(len(data)//k)])
    return smallData

def dynamicProcessAndSorting(k, data):
    # Start all threads.
    process = []
    for n in range(0, k):
        p = multiprocessing.Process(target=bubbleSort(data[n]))
        p.start()
        process.append(p)

    if isDivisible == False: # 額外做一次排序
        p = multiprocessing.Process(target=bubbleSort(data[len(data)-1]))
        p.start()
        process.append(p)

    # Wait all threads to finish.
    for p in process:
        p.join()
    #print(data)

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
    inputFile = open(fileName+".txt","r")
    data = readFile(inputFile)
    if len(data)%k == 0:
        isDivisible = True
    inputFile.close()
    data = devide(data, k)
    start = time.time()
    dynamicProcessAndSorting(k, data)
    mergeCall(data)
    data = dataTemp
    #print(data)
    end = time.time()
    print(end - start)
    #print(data)
    '''
    start = time.time()
    bubbleSort(data)
    end = time.time()
    '''
    #print(end-start)
    outputFile = open(fileName+"_output3.txt","w")
    writeFile(data, outputFile, end-start)




