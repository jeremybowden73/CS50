import nltk

class Analyzer():
    """Implements sentiment analysis.
        using SETS to store the positive and negative words
        from the given text files"""

    def __init__(self, positives, negatives):
        """Initialize Analyzer.
            The "instance variables" created here are not individual
            values for every object created, but are actually empty SETS
            which will contain all the positive and negative words.
            The file "smile" only creates one OBJECT of this class type"""
        self.positives = set()                                  # The sets are instance variables (e.g. self.x) rather than plain sets
        self.negatives = set()                                  # (e.g. x) to make them accessible by other methods in the object.

        with open(positives, 'r') as f:                         # use 'with' to open the file; call the file object 'f'
            for line in f:                                      # for every line in the file object:
                if not line.startswith(";"):                    # if the line DOES NOT start with a ;
                    self.positives.add(line.strip())            # first remove whitespace from the beginning and end of the line,
                                                                # then add the line to the set 'self.positives'

        with open(negatives, 'r') as f:                         # do the same for the negatives file
            for line in f:
                if not line.startswith(";"):
                    self.negatives.add(line.strip())

    def analyze(self, text):
        """Analyze text for sentiment, returning its score.
            The 'searched for' word or string is passed to this method, and this is used in the code
            below to search for it in the two sets created in the __init__ method"""

        self.tknzr = nltk.tokenize.TweetTokenizer()      # OBJECT self.tknzr;   MODULE nltk.tokenize;  CLASS TweetTokenizer
        tokens = self.tknzr.tokenize(text)               # list 'tokens';       METHOD tokenize

        tokens = [x.lower() for x in tokens]            # iterate over the list making all elements lowercase

        # placeholder 'total' keeps track of the overall score based on how many words are in the positive and negative sets
        total = 0

        for i in tokens:
            if i in self.positives:
                total +=1
            elif i in self.negatives:
                total -=1

        return total
