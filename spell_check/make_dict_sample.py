#!/usr/bin/env python3

import os
import argparse
import random

parser = argparse.ArgumentParser(
        description='Output a sample of dictionary words from a source dictionary file.'
        )
parser.add_argument('words', 
        metavar='W', 
        type=int, 
        nargs='?',
        default=100,
        help='an integer for the number of words in the output sample file.'
        )
parser.add_argument('infile', 
        metavar='I', 
        nargs=1,
        type=argparse.FileType('r'),
        help='source file containing a line separated dictionary of words.'
        )
parser.add_argument('outfile', 
        metavar='O',
        nargs='?',
        type=argparse.FileType('w'), 
        default='sample.txt',
        help='destination file including random sample of line separated words from the source.'
        )


if __name__ == '__main__':
    args = parser.parse_args()
    infile = args.infile[-1]
    outfile =args.outfile

    print("Generating sample of %d words from %s" % (args.words, infile.name))
    line_sample = [random.randrange(0, 240000) for x in range(args.words)]

    lines = 0
    sample = []
    for line in infile:
        lines += 1
        if (lines in line_sample):
            sample.append(line)


    shuffle = [random.randrange(0, len(sample)) for x in range(len(sample))]

    for i in range(len(sample)):
        swap = sample[i]
        sample[i] = sample[shuffle[i]]
        sample[shuffle[i]] = swap

    for word in sample:
        outfile.write(word)

    outfile.write('.')

    print("Completed. See %s" % outfile.name)
