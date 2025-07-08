/**
 * Implements the linked list solution to the DNA problem.
 * Provides efficient append and cut/splice operations 
 * Uses optimized traversal approach for chatAt method
 * Supports method to reverse strand while preserving node structure
 * @author Owen Astrachan
 * @author Kavishka Bartlett
 */

public class LinkStrand implements IDnaStrand {
    private class Node {
        String info;
        Node next;
        Node prev;
        Node (String x) {
            info = x;
        }
    }

    private Node myFirst, myLast;
    private long mySize;
    private int myAppends;
    private int myIndex;
    private Node myCurrent;
    private int myLocalIndex; 

    //Defaut Constructor
    public LinkStrand() {
        this("");
    }

    /* 
     *Creates a strand representing strand
     * @param strand
     */
    public LinkStrand(String strand) {
        initialize(strand);
    }

    @Override
    public void initialize(String source) {
        myLast = new Node(source);
        myFirst = myLast;
        mySize = 0;
        myAppends = 0;
        myIndex = 0;
        myLocalIndex = 0;
        myCurrent = myFirst;
        mySize += source.length();
    }
    
    @Override
    public IDnaStrand append(String dna) {
        myLast.next = new Node(dna);
        myLast.next.prev = myLast;
        myLast = myLast.next;
        myAppends++;
        mySize += dna.length();
        return this;
    }

    public int getAppendCount() {
        return myAppends;
    }

    public IDnaStrand getInstance(String source) {
        return new LinkStrand(source);
    }

    public IDnaStrand reverse() {
        Node first = myLast;
      //  StringBuilder reverseFirst = new StringBuilder(first.info);
        //reverseFirst.reverse(); 
        LinkStrand reverseStrand = new LinkStrand(); //reverseFirst.toString()
        while (first != null) {
            StringBuilder temp = new StringBuilder(first.info);
            temp.reverse();
            reverseStrand.append(temp.toString());
            first = first.prev;
            }
        return reverseStrand;
    }

    @Override
    public char charAt(int index) {
        if (index >= mySize || index < 0) 
        throw new IndexOutOfBoundsException("Index " + index + " is out of bounds :(");
        if (index < myIndex || myIndex == 0) { //Check to see if its the first case
            myCurrent = myFirst; //Assign intial values 
            myIndex = 0; 
            myLocalIndex = 0;
        }
            /*While the total index is still greater than the length of the current string,
            loop through the nodes until there is a greater length than index */
            while ((index - myIndex )  > (myCurrent.info.length()-1 - myLocalIndex)) {            
                myIndex += myCurrent.info.length() - myLocalIndex; //Increase index to match how many items gone through
                myCurrent = myCurrent.next;
                myLocalIndex = 0; //No local index if the whole myCurrentlength is in index
            }
        myLocalIndex += index - myIndex;
        myIndex = index; 
        return myCurrent.info.charAt(myLocalIndex);
    }

    public long size() {
        return mySize;
    }

    public String toString() {
        Node temp = myFirst;
        StringBuilder string = new StringBuilder("");
        while (temp != null) {
            string.append(temp.info);
            temp = temp.next;
        }
        return string.toString(); 
    }
}
