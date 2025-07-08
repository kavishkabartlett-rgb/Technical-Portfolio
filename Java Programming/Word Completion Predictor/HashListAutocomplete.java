/**
 * @author Kavishka Bartlett
 * Implementation for autocomplete using a hashmap with O(1) runtime
 * Uses the most memory out of all the algorithms 
 * Stores each prefix (max 10 characters) in HashMap<String, List<Term>>
 * List<Term> is sorted by word weight
*/



import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class HashListAutocomplete implements Autocompletor {

    private static final int MAX_PREFIX = 10;
    private Map<String, List<Term>> myMap;
    private int mySize;
    //private int mySizeTest = 0;

    public HashListAutocomplete(String[] terms, double[] weights) {
        if (terms == null || weights == null) {
			throw new NullPointerException("One or more arguments null");
		}
        if (terms.length != weights.length) {
			throw new IllegalArgumentException("terms and weights are not the same length");
		}
        initialize(terms, weights);
    }

    @Override
    public List<Term> topMatches(String prefix, int k) {
        if (k < 0) {
			throw new IllegalArgumentException("Illegal value of k:"+k);
		}

        if (prefix.length() > MAX_PREFIX) {
            prefix = prefix.substring(0, MAX_PREFIX);
        }
        if (myMap.containsKey(prefix)) {
            List<Term> all = myMap.get(prefix);
            List<Term> list = all.subList(0, Math.min(k, all.size()));
            return list;
        }
        
        return new ArrayList<>();
    }

    @Override
    public void initialize(String[] terms, double[] weights) {
        
        myMap = new HashMap<>();
        int min = 0;

        //creating the map, for each term
        for (int i = 0; i < terms.length; i++)
        {
            min = Math.min(MAX_PREFIX, terms[i].length());
            //for each prefix within each term
            for (int k = 0; k <= min; k++)
            {
                if (!myMap.containsKey(terms[i].substring(0,k))) {
                    //incrememnt mySize for new prefix String created
                    mySize = mySize + (k * BYTES_PER_CHAR);
                    myMap.put(terms[i].substring(0,k), new ArrayList<>());
                }
                myMap.get(terms[i].substring(0,k)).add(new Term(terms[i], weights[i]));
                
                //increment mySize for new term created
                mySize = mySize + (terms[i].length() * BYTES_PER_CHAR) + BYTES_PER_DOUBLE;
            }
        }

        Comparator<Term> comp = Comparator.comparing(Term::getWeight).reversed();
        for (List<Term> list: myMap.values())
        {
            Collections.sort(list, comp);
        }
    }

    @Override
    public int sizeInBytes() {

    /*
    trying non as you go method
    if (mySizeTest == 0) {
        int numchar = 0;
        int numDouble = 0;

        for (String pref: myMap.keySet())
        {
            numchar = numchar + pref.length();
        }

        for (List<Term> list: myMap.values())
        {
            numDouble = numDouble + list.size();
            for (Term t: list)
            {
                numchar = numchar + t.getWord().length();
            }
        }
        mySizeTest = numchar * BYTES_PER_CHAR + numDouble * BYTES_PER_DOUBLE;
    }
    */

    return mySize;

    }
    
}

