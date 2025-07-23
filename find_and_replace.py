import os
import sys

def main():
    if len(sys.argv) != 5:
        print("Usage: python find_and_replace.py srcpath destpath find_path replace_path")
        return

    DEBUG = False

    srcpath = sys.argv[1]
    destpath = sys.argv[2]
    findpath = sys.argv[3]
    find =""
    with open(findpath,"r") as findfile:
        find = findfile.read().strip()

    replacepath = sys.argv[4]
    replace =""
    with open(replacepath,"r") as replacefile:
        replace = replacefile.read().strip()
    if DEBUG:
        print("Replacing [", find, "] with [",replace,"] in [",  srcpath, "] and writing to [", destpath,"].")

    result = ""
    with open(srcpath, "r") as srcfile:
        for line in srcfile:
            result += line.replace(find, replace)
    if DEBUG:
        print("Result of replacement:")
        print(result)
        print("End result")

    with open(destpath, "w") as destfile:
        destfile.write(result)


if __name__ == "__main__":
    main()
