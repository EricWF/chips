#!/usr/bin/python
import sys
import re

old_rows = 16
old_cols = 13

new_rows = 16
new_cols = 8

old_to_new = dict()


def create_old_to_new():
    for old_index in range(0, old_rows * old_cols):
            old_x = old_index % old_cols
            old_y = old_index / old_cols
            new_index = (old_x + (old_y * new_cols))
            old_to_new[old_index] = new_index
            
def transform_tileset(data):
    for k in old_to_new:
        old_str = ' id="' + str(k) + '"'
        new_str = ' tmpid="' + str(old_to_new[k]) + '"'
        data = re.sub(old_str, new_str, data)
    data = re.sub("tmpid=","id=", data)
    return data

def transform_data(data):
    for k in old_to_new:
        old_str = ' gid="' + str(k + 1) + '"'
        new_str = ' tmpgid="' + str(old_to_new[k] + 1) + '"'
        data = re.sub(old_str, new_str, data)
    data = re.sub("tmpgid=","gid=", data)
    return data

def write_data(data):
    new_fname = sys.argv[2]
    print("Writing to file: " + new_fname)
    with open(new_fname, 'w') as f:
        f.write(data)


def main():
    create_old_to_new()
    print("Using file: " + str(sys.argv[1]))
    with open(sys.argv[1], 'r') as f:
        data = f.read()
        tdata = transform_data(data)
        #tdata = transform_tileset(data)
        write_data(tdata)
    

if __name__ == '__main__':
    main()