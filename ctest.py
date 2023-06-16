import os
import sys
import time


if "testcpp.exe" in os.listdir():
    os.remove("testcpp.exe")
    print("Removed testcpp")

# can pass -d flag for just delete, and not this next bit
if "-d" not in sys.argv:
    print("Compiling testcpp")
    os.system("g++ -o testcpp src/test.cpp")
    # let it compile - hopefully
    time.sleep(1)
    if "testcpp.exe" in os.listdir():
        print("Found file; output-------------------------------------------")
        os.system("testcpp.exe")
        print("\nEnd output ------------------------------------------------")
