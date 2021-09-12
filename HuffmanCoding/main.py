# 演算法分析機測
# 學號 : 10727252
# 姓名 : 黃于九
# 中原大學資訊工程系
temp = None
class Node:
    def __init__(self, name, frequency):
        self.name = name
        self.frequency = frequency
        self.left = None
        self.right = None
        self.back = None
    def setLeft(self, left):
        self.left = left
    def setRight(self, right):
        self.right = right
    def setBack(self, back):
        self.back = back

def bubbleSort( pairs ):
    n = len( pairs )
    for i in range( 0, n - 1 ):
        for j in range( i + 1, n ):
            if pairs[i].frequency > pairs[j].frequency:
                pairs[i], pairs[j] = pairs[j], pairs[i]
    return pairs

def creatHuffmanTree(n, pairs):
    pairs = bubbleSort(pairs)
    for i in range(n-1):
        node1 = pairs[0]
        node2 = pairs[1]

        root = Node(None, pairs[0].frequency+pairs[1].frequency)
        root.left = node1
        root.right = node2
        node1.back = root
        node2.back = root
        pairs.pop(0)
        pairs.pop(0)
        pairs.append(root)
        pairs = bubbleSort(pairs)

    return pairs

def findNode(root, data):
    global temp
    if root == None:
        return
    findNode(root.left, data)
    findNode(root.right, data)
    if root.name == data[0]:
        temp = root

def findCode():
    global temp
    s = ""
    while temp.back != None:
        if temp.back.left == temp:
            s = "0" + s
            temp = temp.back
        else:
            s = "1" + s
            temp = temp.back
    return s

def encodes(tree, table):
    global temp
    for i in range(len(table)):
        temp = None
        findNode(tree[0], table[i]) # 會設定temp指向目標字母
        s = findCode()
        table[i][2] = s
    return table

if __name__ == '__main__':
    tables = []
    while (1):
        nameList = []
        frequenceList = []
        pairs = []
        n = input()
        if n == "0":
            break

        for i in range(int(n)):
            temp = [x for x in input().split()]
            nameList.append(temp[0])
            frequenceList.append(int(temp[1]))

        for i in range(int(n)):
            node = Node(nameList[i], frequenceList[i])
            pairs.append(node)

        table = []
        for i in range(len(pairs)):
            table.append([pairs[i].name, pairs[i].frequency, "-1"])
            #print(table[i])

        pairs = bubbleSort(pairs)
        tree = creatHuffmanTree(int(n), pairs)
        table = encodes(tree, table)
        tables.append(table)

    for i in range(len(tables)):
        print("Huffman Codes #" + str(i+1))
        for j in range(len(tables[i])):
            print(tables[i][j][0], tables[i][j][2])
