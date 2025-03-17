/**
 * A WordGram object represents an immutable
 * sequence of words.
 * @author Brandon Fain
 * @author Kavishka Bartlett
 */

public class WordGram {
	private String[] myWords; 	// Stores WordGram words
	private String myToString;	// Stores space separated words as one string
	private int myHash;			// Stores hash value of WordGram

	
	/**
	 * Constructor should generate a WordGram with size words
	 * beginning at the start index value of source array.
	 * Each element of source array should be a single word.
	 * @param source Source array, each element should be a single word
	 * @param start Index of first word for WordGram object
	 * @param size Number of elements in WordGram object
	 */
	public WordGram(String[] source, int start, int size) {
        myWords = new String[size];
        myToString = "";
        myHash = 0;
        for(int i = 0; i<size; i++){
            if(i+start>source.length-1){
                myWords[i] = " ";
            }
            else{
            myWords[i] = source[i+start];
            }
        }

    }

	/**
	 * Return the word at a given index of WordGram
	 * @param index of word
	 * @return String/word at index position in WordGram
	 * @throws IndexOutOfBoundsException if index < 0 or index >= length()
	 */
	public String wordAt(int index) {
		return myWords[index];
	}

	/**
	 * Returns number of words in this WordGram
	 * @return order of wordgram, number of words
	 */
	public int length() {
		return myWords.length;
	}

	/** 
	 * Returns true if o is also a wordgram with the
	 * same words, otherwise returns false 
	*/
	@Override
	public boolean equals (Object o) {
		if (! (o instanceof WordGram) || o == null){
			return false;
		}
		WordGram other = (WordGram) o;
		if (!(this.length() == other.length())) {
			return false;
		}
		for (int i=0; i<myWords.length; i++) {
			if (!(this.myWords[i].equals(other.myWords[i]))) {
			return false;
			}
		}
		return true;
	}

	/**
	 * Returns a hashCode for WordGram object equal to 
	 * the hashCode of the space separated words stored in 
	 * the WordGram.
	 */
	@Override
	public int hashCode() {
		if (myHash == 0)
		myHash = this.toString().hashCode();
		return myHash;
	}


	/**
	 * Return a new WordGram of the same length as this WordGram 
	 * consisting of words 1 through length-1 of this WordGram
	 * followed by last. Does NOT mutate this WordGram.
	 * @param last added as last string of returned WordGram
	 * Should be a single word
	 * @return new WordGram
	 */
	public WordGram shiftAdd(String last) {
		String[] newWordGram = new String[this.length()];
		for (int k=0; k<this.length(); k++) {
			if (k+1<this.length())
				newWordGram[k] = this.wordAt(k+1);
			newWordGram[this.length() - 1] = last;
		}
		WordGram newObj = new WordGram(newWordGram, 0, newWordGram.length);
		return newObj;
	}

	/**
	 * Returns space separated words stored in the WordGram
	 * as a single String.
	 */
	@Override
	public String toString() {
		if (myToString == "") {
			myToString = String.join(" ", myWords);
		}
		return myToString;
	}
}