import first
import second
import third
import fourth
import fiveth

def writeFile( dataTemp, answer, waitingTime, turnaroundTime, outputFile ):
    outputFile.write("All" + "\n")
    outputFile.write("==        FCFS==" + "\n")
    for i in answer[0]:
        outputFile.write(str(i))
    outputFile.write("\n")

    outputFile.write("==          RR==" + "\n")
    for i in answer[1]:
        outputFile.write(str(i))
    outputFile.write("\n")

    outputFile.write("==        SRTF==" + "\n")
    for i in answer[2]:
        outputFile.write(str(i))
    outputFile.write("\n")

    outputFile.write("==        PPRR==" + "\n")
    for i in answer[3]:
        outputFile.write(str(i))
    outputFile.write("\n")

    outputFile.write("==        HRRN==" + "\n")
    for i in answer[4]:
        outputFile.write(str(i))
    outputFile.write("\n")

    outputFile.write("===========================================================" + "\n")
    outputFile.write("\n")

    outputFile.write("waiting" + "\n")
    outputFile.write("ID" + "\t" + "FCFS" + "\t" + "RR" + "\t" + "SRTF" + "\t" + "PPRR" + "\t" + "HRRN" + "\n")
    outputFile.write("===========================================================" + "\n")
    for i in range(len(dataTemp[0])):
        outputFile.write(str(dataTemp[0][i][0]) + "\t")
        outputFile.write(str(waitingTime[0][i]) + "\t" + str(waitingTime[1][i]) + "\t" + str(waitingTime[2][i]) + "\t" + str(waitingTime[3][i]) + "\t" + str(waitingTime[4][i]) + "\n")
    outputFile.write("===========================================================" + "\n")
    outputFile.write("\n")
    outputFile.write("Turnaround Time" + "\n")
    outputFile.write("ID" + "\t" + "FCFS" + "\t" + "RR" + "\t" + "SRTF" + "\t" + "PPRR" + "\t" + "HRRN" + "\n")
    outputFile.write("===========================================================" + "\n")
    for i in range(len(dataTemp[0])):
        outputFile.write(str(dataTemp[0][i][0]) + "\t")
        outputFile.write(str(turnaroundTime[0][i]) + "\t" + str(turnaroundTime[1][i]) + "\t" + str(turnaroundTime[2][i]) + "\t" + str(turnaroundTime[3][i]) + "\t" + str(turnaroundTime[4][i]) + "\n")
    outputFile.write("===========================================================" + "\n")
    outputFile.write("\n")

#if __name__ == '__main__':
def main(fileName):
    dataTemp = []
    answer = []
    waitingTime = []
    turnaroundTime = []
    #print("請輸入檔案名稱：(副檔名是.txt)")
    #fileName = input()

    temp1, temp2, temp3, temp4 = first.runAll(fileName)
    dataTemp.append(temp1)
    answer.append(temp2)
    waitingTime.append(temp3)
    turnaroundTime.append(temp4)
    #print(temp2)
    temp1, temp2, temp3, temp4 = second.runAll(fileName)
    dataTemp.append(temp1)
    answer.append(temp2)
    waitingTime.append(temp3)
    turnaroundTime.append(temp4)
    #print(2)
    temp1, temp2, temp3, temp4 = third.runAll(fileName)
    dataTemp.append(temp1)
    answer.append(temp2)
    waitingTime.append(temp3)
    turnaroundTime.append(temp4)
    #print(3)
    temp1, temp2, temp3, temp4 = fourth.runAll(fileName)
    dataTemp.append(temp1)
    answer.append(temp2)
    waitingTime.append(temp3)
    turnaroundTime.append(temp4)
    #print(4)
    temp1, temp2, temp3, temp4 = fiveth.runAll(fileName)
    dataTemp.append(temp1)
    answer.append(temp2)
    waitingTime.append(temp3)
    turnaroundTime.append(temp4)
    #print(5)
    outputFile = open("out_" + fileName + ".txt", "w")
    writeFile(dataTemp, answer, waitingTime, turnaroundTime, outputFile)
