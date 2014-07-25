Cornell Tech Self Assessment
=========================
Overview
---------
This repo includes work for the Cornell Tech Connective Media programming self-assessment.

Assignment
----------
### Cornell Tech self-assessment programming exercises for incoming masters students

You can do these programming assignments in any language, but we suggest (in alphabetical order) that you use C, Java or Python. Variants of these languages, such as Objective C, are fine as well.

1. Write a program that takes a text file as input and produces a file as output with the input reversed. Assume the input has no punctuation, and is all lower case.
    1. The 1st version should do reversal at the level of characters. If the input file contains “more mor naaman” the output file should contain “namaan rom erom”.
    2. The 2nd version should do reversal at the level of words. If the input file contains “more mor naaman” the output file should contain “naaman mor more”.
    3. The 3rd version should do reversal at the level of characters within each word. If the input file contains “more mor namaan” the output file should contain “erom rom namaan”.


2. Write a program that detects palindromes. Again, assume the input has no punctuation, and is all lower case. Print out a message that tells the user whether the input is or is not a palindrome.

3. Write a simple spelling checker. Your dictionary will consist of the words at http://svnweb.freebsd.org/base/head/share/dict/web2?revision=213061&view=co. You will create a user interface where the user types in a word and is told whether or not the word is spelled correctly. The user can then type in another word, or can type in a period by itself which will cause the program to exit. Again you can ignore case (i.e. “Cat” and “austria” are both spelled correctly). You can use built-in data structures, or you can write them yourself.
    1. In the 1st version the dictionary should be implemented using a linked list.
    2. In the 2nd version the dictionary should be implemented using a splay tree.
    3. In the 3rd version the dictionary should be implemented using a hash table.
    4. Provide a chart of running time for each of your versions, divided into time to read in the dictionary versus time to process each query. You can use very coarse timings.
