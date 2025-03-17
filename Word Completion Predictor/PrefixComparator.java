import java.util.Comparator;

/**
 * Factor pattern for obtaining PrefixComparator objects
 * without calling new. Users simply use
 *
 *     Comparator<Term> comp = PrefixComparator.getComparator(size)
 *
 * @author owen astrachan
 * @date October 8, 2020
 * @author Kavishka Bartlett
 */
public class    PrefixComparator implements Comparator<Term> {

    private int myPrefixSize; // size of prefix

    /**
     * private constructor, called by getComparator
     * @param prefix is prefix used in compare method
     */
    private PrefixComparator(int prefix) {
        myPrefixSize = prefix;
    }


    /**
     * Factory method to return a PrefixComparator object
     * @param prefix is the size of the prefix to compare with
     * @return PrefixComparator that uses prefix
     */
    public static PrefixComparator getComparator(int prefix) {
        return new PrefixComparator(prefix);
    }


    @Override
    /**
     * Use at most myPrefixSize characters from each of v and w
     * to return a value comparing v and w by words. Comparisons
     * should be made based on the first myPrefixSize chars in v and w.
     * @return < 0 if v < w, == 0 if v == w, and > 0 if v > w
     */
    public int compare(Term v, Term w) {
        int index = 0;
        int i = 0;

        if (v.getWord().length() < myPrefixSize || w.getWord().length() < myPrefixSize) {
            while(i == 0 && index < v.getWord().length() && index < w.getWord().length())
            {
                if (v.getWord().charAt(index) < w.getWord().charAt(index)) {
                    i = -1;
                }
                if (v.getWord().charAt(index) > w.getWord().charAt(index)) {
                    i = 1;
                }
                index++;
            }
            if (i == 0)
            {
                i = v.compareTo(w);
            }
        }
        else {
            while(i == 0 && index < myPrefixSize)
            {
               if (v.getWord().charAt(index) < w.getWord().charAt(index)) {
                    i = -1;
               }
                if (v.getWord().charAt(index) > w.getWord().charAt(index)) {
                    i = 1;
                }
                index++;
            }
        }

        return i;
    }
}
