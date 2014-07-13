"""Reverser.

Usage:
    reverser.py <file> [--] (--o | --c | --w | --wc)

Options:
    --o     Print the original contents of the file.
    --c     Print the file's Characters in reverse.
    --w     Print the file's Words in reverse.
    --wc    Print the file's Words with the Characters in reverse.

"""

from docopt import docopt
import os


class Reverser(object):
    """Reverses text from an input file using 1 of 3 methods.
        1) characters
    """

    @classmethod
    def _get_input_text(cls, filename):
        with open(filename, 'r') as infile:
            return ''.join(infile.readlines())

    @classmethod
    def _get_lines(cls, filename):
        with open(filename, 'r') as infile:
            return infile.readlines()

    def __init__(self, filename):
        self._input_text = Reverser._get_input_text(filename)
        self._input_lines = Reverser._get_lines(filename)
        self._character_count = len(self._input_text)

    def original(self):
        print(self._input_text)

    def reverse_characters(self):
        """Print the input's characters in reverser order.
        (e.g., dlrow olleH)"""
        for i in range(len(self._input_lines) - 1, -1, -1):
            line = self._input_lines[i]
            for j in range(len(line) - 1, -1, -1):
                print(line[j], end='')
                #print(self._input_text[i], end='')
        print()

    def reverse_words(self):
        """Print the input's words in reverser order. (e.g., world Hello)"""
        for i in range(len(self._input_lines) - 1, -1, -1):
            words = self._input_lines[i].split()
            output = ' '.join([words[j] for j in range(len(words) - 1, -1, -1)])
            print(output)

    def reverse_word_characters(self):
        """Print the input's words in order, but with each word's characters
        in reverser order. (e.g., olleH dlroW)"""
        for line in self._input_lines:
            words = line.split()
            output = []

            for word in words:
                backwards = ''.join(word[i] for i in
                                    range(len(word) - 1, -1, -1))
                output.append(backwards)

            print(' '.join(output))


if __name__ == '__main__':
    arguments = docopt(__doc__)
    #print(arguments, end='\n\n')
    filename = arguments.get('<file>')

    reverser = None
    try:
        reverser = Reverser(filename)
    except IOError as e:
        print("Cannot open file %s\n" % filename)
        exit(1)

    print()
    if arguments.get('--o'):
        reverser.original()
    if arguments.get('--c'):
        reverser.reverse_characters()
    if arguments.get('--w'):
        reverser.reverse_words()
    if arguments.get('--wc'):
        reverser.reverse_word_characters()
    print()
