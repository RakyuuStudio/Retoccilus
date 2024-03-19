# Note: This program is just an example. It does not have any practical use and is only for demonstration purposes.
# It will not pass the compile I think
def parseIn():
    sortList = []
    for _ in range(10):
        x = int(input())
        sortList.append(x)
    return sortList

def compare(a, b):
    return a > b

def core(sort_list):
    sort_list.sort(key=compare, reverse=True)

def write_out(sort_list):
    for i in sort_list:
        print(i, end="")
    print()

if __name__ == "__main__":
    sort_list = parseIn()
    core(sort_list)
    write_out(sort_list)