#!/usr/bin/env python3

import os
import sys


class ArrayCompare():
    def __init__(self, fname):
        self.fname = fname
        os.path.exists(self.fname)

    def compare(self):
        try:
            with os.popen('./%s' % self.fname) as f:
                data = f.read()
                return data
        except Exception as e:
            print(e)
            input('Please input any key for exitting:')
            sys.exit(1)


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Input parameters for one, please check your parameter!")
        sys.exit(1)

    fname = sys.argv[1]

    ac = ArrayCompare(fname)
    data = ac.compare()
    data = data.split()
    key = data[0]
    for d in data:
        if key == d:
            print(d, end=' ')
        else:
            print('\033[0;31m%s\033[0m' % d, end=' ')
    print()
