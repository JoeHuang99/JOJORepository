
class Process: # 負責單個Process的狀態
    def __init__(self, ID, cpuBurst, arrivalTime, priority):
        self.ID = ID
        self.cpuBurst = cpuBurst
        self.arrivalTime = arrivalTime
        self.priority = priority

class State: # 負責整體的狀態
    def __init__(self):
        self.time = 0
        self.cpuProcess = Process(-1, -1, -1 ,-1)
        self.readyQueue = []
        self.answer = []
    def pushJobInReadyQueue(self, data):
        while data[0].arrivalTime == self.time:
            self.readyQueue.append(data[0])
            data.pop(0)
            if len(data) == 0:
                break

    def getNextJob(self):
        self.cpuProcess = self.readyQueue[0]
        self.readyQueue.pop(0)

    def schedual(self):
        if self.cpuProcess.ID == -1:
            self.getNextJob()
        elif self.cpuProcess.cpuBurst == 0:
            self.getNextJob()
        else:
            pass

    def checkChar(self, number):
        if number < 10:
            return number
        else:
            x = number + 55
            x = chr(x)
            return x

    def run(self):

        if self.cpuProcess.cpuBurst <= 0:
            self.answer.append('-')
        else:
            #print(self.cpuProcess.ID)
            self.answer.append(self.checkChar(self.cpuProcess.ID))
            self.cpuProcess.cpuBurst = self.cpuProcess.cpuBurst - 1

def readFile( file ):
    data = []
    for i in file.readlines():
        data.append(i.strip('\n'))
    return data

def getLineData(data):
    temp = []
    s = ""
    x = 0
    for i in range(len(data)):
        if data[i] != ' ' and data[i] != "\t":
            s = s + data[i]
            x = 1
        if (data[i] == ' ' or data[i] == "\t") and x == 1:
            temp.append(int(s))
            s = ""
            x = 0
    if s != "":
        temp.append(int(s))
    return temp

def splitData(data):
    choose = data[0][0]
    timeSlice = ""
    for i in range(len(data[0]) - 1, 0, -1):
        if data[0][i] != ' ' and data[0][i] != "\t":
            timeSlice = data[0][i] + timeSlice

    dataList = []
    for i in range(2, len(data), 1):
        temp = getLineData(data[i])
        #temp = list(map(int, temp)) # 將List中的String轉為Int
        #print(temp)
        if temp != []:
            p = Process(temp[0], temp[1], temp[2], temp[3])
            dataList.append(p)
        #print(temp)
    return choose, timeSlice, dataList

def dataSort(data):
    n = len(data)
    for i in range(0, n - 1, 1):
        for j in range(n - 1, i, -1):
            if data[j].arrivalTime < data[j - 1].arrivalTime:
                data[j], data[j - 1] = data[j - 1], data[j]
            elif data[j].arrivalTime == data[j - 1].arrivalTime:
                if data[j].ID < data[j - 1].ID:
                    data[j], data[j - 1] = data[j - 1], data[j]
    return data

def dataSort2(data):
    n = len(data)
    for i in range(0, n - 1, 1):
        for j in range(n - 1, i, -1):
            if data[j][0] < data[j - 1][0]:
                data[j], data[j - 1] = data[j - 1], data[j]
            elif data[j][0] == data[j - 1][0]:
                if data[j].ID < data[j - 1].ID:
                    data[j], data[j - 1] = data[j - 1], data[j]
    return data

def getTotalCPUBurst(data):
    total = 0
    for i in range(len(data)):
        total = total + data[i].cpuBurst
    return total

def writeFile( dataTemp, answer, waitingTime, turnaroundTime, outputFile ):
    outputFile.write("FCFS" + "\n")
    for i in answer:
        outputFile.write(str(i))
    outputFile.write("\n")
    outputFile.write("===========================================================" + "\n")
    outputFile.write("\n")
    outputFile.write("waiting" + "\n")
    outputFile.write("ID" + "\t" + "FCFS" + "\n")
    outputFile.write("===========================================================" + "\n")
    for i in range(len(dataTemp)):
        outputFile.write(str(dataTemp[i][0]) + "\t" + str(waitingTime[i]) + "\n")
    outputFile.write("===========================================================" + "\n")
    outputFile.write("\n")
    outputFile.write("Turnaround Time" + "\n")
    outputFile.write("ID" + "\t" + "FCFS" + "\n")
    outputFile.write("===========================================================" + "\n")
    for i in range(len(dataTemp)):
        outputFile.write(str(dataTemp[i][0]) + "\t" + str(turnaroundTime[i]) + "\n")

def charToInt(ch):
    if ch == '-':
        return -1
    elif type(ch) == int:
        return ch
    else:
        number = ord(ch) - 55
        return number

def computeWattingTime(data, answer):
    waitingTime = []
    for i in range(len(data)):
        id = data[i][0]
        cpuBrust = data[i][1]
        arrivalTime = data[i][2]
        for j in range(len(answer)-1, -1, -1):
            if charToInt(answer[j]) == id:
                index = j + 1
                temp = index - cpuBrust - arrivalTime
                waitingTime.append(temp)
                #print(temp)
                break
    return waitingTime
def computeTurnaroundTime(data, answer):
    turnaroundTime = []
    for i in range(len(data)):
        id = data[i][0]
        arrivalTime = data[i][2]
        for j in range(len(answer) - 1, -1, -1):
            if charToInt(answer[j]) == id:
                index = j + 1
                temp = index - arrivalTime
                turnaroundTime.append(temp)
                #print(temp)
                break
    return turnaroundTime

def dataTranfer(data):
    dataTemp = []
    for i in range(len(data)):
        temp = []
        temp.append(data[i].ID)
        temp.append(data[i].cpuBurst)
        temp.append(data[i].arrivalTime)
        temp.append(data[i].priority)
        dataTemp.append(temp)
    dataTemp = dataSort2(dataTemp)
    return dataTemp

def runAll(fileName):
    inputFile = open(fileName + ".txt", "r")
    data = readFile(inputFile)
    choose, timeSlice, data = splitData(data)  # data中所存的是Process物件
    data = dataSort(data)  # 將data依照Arrival Time排序
    dataTemp = dataTranfer(data)
    #print(dataTemp)
    state = State()
    totalCPUBurst = getTotalCPUBurst(data)
    #print(totalCPUBurst)
    nowTime = 0
    while (nowTime < totalCPUBurst):
        if len(data) != 0:
            state.pushJobInReadyQueue(data)
        if len(state.readyQueue) != 0:
            state.schedual()
        state.run()
        state.time = state.time + 1
        # print(state.answer)
        nowTime = nowTime + 1
        if state.answer[-1] == '-':
            nowTime = nowTime - 1

    #print(dataTemp)
    waitingTime = computeWattingTime(dataTemp, state.answer)
    turnaroundTime = computeTurnaroundTime(dataTemp, state.answer)
    return dataTemp, state.answer, waitingTime, turnaroundTime

#if __name__ == '__main__':
def main(fileName):
    inputFile = open(fileName + ".txt", "r")
    data = readFile(inputFile)
    choose, timeSlice, data = splitData(data) # data中所存的是Process物件
    data = dataSort(data) # 將data依照Arrival Time排序
    dataTemp = dataTranfer(data)
    state = State()
    totalCPUBurst = getTotalCPUBurst(data)
    #print(totalCPUBurst)
    nowTime = 0
    while(nowTime < totalCPUBurst):
        if len(data) != 0:
            state.pushJobInReadyQueue(data)
        if len(state.readyQueue) != 0:
            state.schedual()
        state.run()

        state.time = state.time + 1
        nowTime = nowTime + 1
        if state.answer[-1] == '-':
            nowTime = nowTime - 1
        #print(state.answer)

    #print(dataTemp)
    waitingTime = computeWattingTime(dataTemp, state.answer)
    turnaroundTime = computeTurnaroundTime(dataTemp, state.answer)
    #print(state.answer)
    outputFile = open("out_" + fileName + ".txt", "w")
    writeFile(dataTemp, state.answer, waitingTime, turnaroundTime, outputFile)