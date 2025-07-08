/**
 * Uses a HashMap Markov model to generate text
 * The keys are WordGrams while the values are possible next words
 * Efficiently generates random text due to O(1) access time for values in a HashMap
 * 
 * @author CS 201 Staff
 * @author Kavishka Bartlett
 */


import java.util.*;

public class HashMarkov implements MarkovInterface {

    protected String[] myWords;     // Training text split into array of words 
    protected Random myRandom;      // Random number generator
    protected int myOrder;          // Length of WordGrams used
    protected HashMap <WordGram, ArrayList<String>> map;

    public HashMarkov(int order){
        myOrder = order;
        myRandom = new Random();
        map = new HashMap<>();
    }
    public int getOrder() {
        return myOrder;
    }
    public void setSeed(long seed) {
        myRandom.setSeed(seed);
    } 
    public void setTraining(String text){
        map.clear();
        myWords = text.split("\\s+"); 
        for(int i = 0; i<myWords.length - myOrder; i++){
            String words = myWords[i+myOrder];
            WordGram addGram = new WordGram(myWords, i, myOrder);
            ArrayList<String> array = new ArrayList<>();
            if(map.containsKey(addGram)){
                array = map.get(addGram);
                array.add(words);
            }
            else {
                ArrayList<String> array2 = new ArrayList<>();
                array2.add((words));
                map.put(addGram, array2);
            }
        }
    }
    public List<String> getFollows(WordGram wgram) {
        List<String> list = new ArrayList<>();
        if(map.containsKey(wgram)){
            list = map.get(wgram);
        }
        return list;
    }
    private String getNext(WordGram wgram) {
        List<String> list = getFollows(wgram);
        if (list.size() == 0) {
            int randomIndex = myRandom.nextInt(myWords.length);
            list.add(myWords[randomIndex]);
        }
        int randomIndex = myRandom.nextInt(list.size());
        return list.get(randomIndex);
    }
    public String getRandomText(int length){
        ArrayList<String> randomWords = new ArrayList<>(length);
        int index = myRandom.nextInt(myWords.length - myOrder + 1);
        WordGram current = new WordGram(myWords,index,myOrder);
        randomWords.add(current.toString());
        for(int k=0; k < length-myOrder; k += 1) {
            String nextWord = getNext(current); //get next calls get list - so its more efficient
            randomWords.add(nextWord);
            current = current.shiftAdd(nextWord);
        }
        return String.join(" ", randomWords);
    }
}

    
    

