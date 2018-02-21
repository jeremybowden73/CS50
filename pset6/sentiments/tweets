#!/usr/bin/env python3

import os
import sys

from analyzer import Analyzer                       # MODULE analyzer;   CLASS Analyzer
from termcolor import colored                       # MODULE termcolor;  FUNCTION colored
from helpers import get_user_timeline

def main():

    # ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./smile @screen_name")

    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # create a list of most recent 50 tweets using the @screen_name
    # entered at the command line argument and
    # the method get_user_timeline from helpers.py
    tweets = get_user_timeline(sys.argv[1], 50)
    
    # check if the list is empty. An empty list returns FALSE, so it's easy to do.
    if not tweets:
        sys.exit("Sorry, that username is either non-existent or private.")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)               # OBJECT analyzer;  CLASS Analyzer

    # analyze the list of tweets (list 'tweets')
    # for each tweet, the same object ('analyzer') is used; all that changes
    # each iteration is the string from the list that is passed to the method
    for i in tweets:
        score = analyzer.analyze(i)                         # OBJECT analyzer;  METHOD analyze
        if score > 0.0:
            print(colored(" 1 {}".format(i), "green"))
        elif score < 0.0:
            print(colored("-1 {}".format(i), "red"))
        else:
            print(colored(" 0 {}".format(i), "yellow"))
    
if __name__ == "__main__":
    main()
