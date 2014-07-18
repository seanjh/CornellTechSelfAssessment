
class Palindrome(object):
    def __init__(self):
        self._left_chars = []
        self._right_chars = []

    @property
    def character_count(self):
        if (not self):
            return len(self._left_chars) + len(self._right_chars)
        else:
            return None

    def __bool__(self):
        if self._left_chars:
            return True
        else:
            return False

    def append_pair(self, new_left, new_right):
        self._left_chars.append(new_left)
        self._right_chars.insert(0, new_right)

    def append_middle(self, char):
        # Add the remaining middle character for palindromes with an odd
        # number of total characters
        self._left_chars.append(char)

    def __str__(self):
        return ''.join(self._left_chars) + ''.join(self._right_chars)

    def __repr__(self):
        return ("Palindrome()\n\telements: %s" % 
            (self._left_chars + self._right_chars))


class PalindromeFinder(object):

    def __init__(self, string):
        self.char_list = list(string)
        self.palindrome = None
        self._find_palindrome()

    @property
    def source_string(self):
        return ''.join(self.char_list)
    @source_string.setter
    def source_string(self, value):
        self.char_list = list(value)

    def _find_palindrome(self):
        left = 0
        right = len(self.char_list) - 1
        pal = Palindrome()

        while (left < right):
            left_char = self.char_list[left]
            right_char = self.char_list[right]

            #print("left: %s (%d), right: %s (%d)" % (left_char, left, right_char, right))

            if (left_char == right_char):
                pal.append_pair(left_char, right_char)
            else:
                return

            left += 1
            right -= 1
            #print("pal: %s" % repr(pal))

        if (left == right) and pal:
            pal.append_middle(self.char_list[left])

        self.palindrome = pal

    def _check_pairs(self, palindrome):
        pass


def get_input():
    return input("Please enter a string (X to exit): ")


if __name__ == '__main__':
    print("\nThis program checks input strings for palindromes.")
    print("Note: Punctuation and spaces are not supported.\n")

    source = get_input()
    while (source.lower() != 'x'):
        finder = PalindromeFinder(source)
        if (finder.palindrome):
            print("%s is a palindrome!" % finder.palindrome)
        else:
            print("%s is not a palindrome!" % source)
        source = get_input()
