import first
import second
import third
import fourth

if __name__ == '__main__':
    while(1):
        print("請輸入任務編號：")
        print("任務一(1)")
        print("任務二(2)")
        print("任務三(3)")
        print("任務四(4)")
        print("離開(0)")
        choose = input()
        if choose == '0':
            break
        elif choose == '1':
            first.main()
        elif choose == '2':
            second.main()
        elif choose == '3':
            third.main()
        elif choose == '4':
            fourth.start()
        else:
            continue

