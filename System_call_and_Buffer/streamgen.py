import random
import string

def randomString(len=10):
    strea = ''.join([random.choice(" "+string.ascii_letters + string.digits) for n in range(len)])
    return strea

def main():
    print(randomString(1000), end = "")

if __name__=='__main__':
    main()