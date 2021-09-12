import first
import second
import third
import fourth
import fiveth
import sixth

def readFile( file ):
    data = []
    for i in file.readlines():
        data.append(i.strip('\n'))
    return data

def splitData(data):
    choose = data[0][0]
    return choose

def getChoose():
    print("請輸入檔案名稱：(副檔名是.txt)")
    fileName = input()
    if fileName == '0':
        choose = '0'
        return choose, None
    inputFile = open(fileName + ".txt", "r")
    data = readFile(inputFile)
    choose = splitData(data)  # data中所存的是Process物件
    return choose, fileName

if __name__ == '__main__':
    while(1):
        choose, fileName = getChoose()
        if choose == '0':
            break;
        elif choose == '1':
            first.main(fileName)
        elif choose == '2':
            second.main(fileName)
        elif choose == '3':
            third.main(fileName)
        elif choose == '4':
            fourth.main(fileName)
        elif choose == '5':
            fiveth.main(fileName)
        elif choose == '6':
            sixth.main(fileName)
        else:
            continue