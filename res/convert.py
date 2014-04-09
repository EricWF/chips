#!/usr/bin/python
import sys
import re

old_rows = 16
old_cols = 13

new_rows = 16
new_cols = 8

old_to_new = dict()

str_key=" gid="
tmp_key=" tmpgid="


def old_id_to_loc(x):
    p = (x % old_cols), (x / old_cols)
    return p

def loc_to_new_id(l):
    return l[0] + (l[1] * new_cols)

def create_old_to_new():
    for x in range(0, new_cols):
        for y in range(0, new_rows):
            old_id = (x + (old_cols * y)) 
            new_id = (x + (new_cols * y))
            old_to_new[old_id] = new_id
    print(old_to_new)
            

def transform_data(data):
    for k in old_to_new:
        old_str = ' id="' + str(k + 1) + '"'
        new_str = ' tmpid="' + str(old_to_new[k] + 1) + '"'
        data = re.sub(old_str, new_str, data)
    data = re.sub("tmpid=","id=", data)
    return data

def write_data(data):
    new_fname = sys.argv[1] + "_NEW"
    print("Writing to file: " + new_fname)
    with open(new_fname, 'w') as f:
        f.write(data)


def main():
    create_old_to_new()
    print("Using file: " + str(sys.argv[1]))
    with open(sys.argv[1], 'r') as f:
        data = f.read()
        tdata = transform_data(data)
        write_data(tdata)
    

if __name__ == '__main__':
    main()