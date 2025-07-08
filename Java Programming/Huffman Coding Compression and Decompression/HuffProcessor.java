import java.util.PriorityQueue;

/**
 * Although this class has a history of several years,
 * it is starting from a blank-slate, new and clean implementation
 * as of Fall 2018.
 * <P>
 * Changes include relying solely on a tree for header information
 * and including debug and bits read/written information
 * 
 * @author Ow	en Astrachan
 * @author Kavishka Bartlett
 *
 * Revise
 */

public class HuffProcessor {

	//Class to represent and define the implementation of a node in the Huffman tree 
	private class HuffNode implements Comparable<HuffNode> {
		HuffNode left;
		HuffNode right;
		int value;
		int weight;

		public HuffNode(int val, int count) {
			value = val;
			weight = count;
		}
		public HuffNode(int val, int count, HuffNode ltree, HuffNode rtree) {
			value = val;
			weight = count;
			left = ltree;		//Used to identify student nodes
			right = rtree;
		}

		//Priority queue behavior
		public int compareTo(HuffNode o) {
			return weight - o.weight;		//Nodes are compared using frequency
		}
	}

	public static final int BITS_PER_WORD = 8;
	public static final int BITS_PER_INT = 32;
	public static final int ALPH_SIZE = (1 << BITS_PER_WORD); 
	public static final int PSEUDO_EOF = ALPH_SIZE;
	public static final int HUFF_NUMBER = 0xface8200;
	public static final int HUFF_TREE  = HUFF_NUMBER | 1;

	private boolean myDebugging = false;
	
	public HuffProcessor() {
		this(false);
	}
	
	public HuffProcessor(boolean debug) {
		myDebugging = debug;
	}

	/**
	 * Compresses a file. Process must be reversible and loss-less.
	 *
	 * @param in
	 *            Buffered bit stream of the file to be compressed.
	 * @param out
	 *            Buffered bit stream writing to the output file.
	 */
	public void compress(BitInputStream in, BitOutputStream out){
		int[] counts = getCounts(in);			//First count character frequencies
		HuffNode root = makeTree(counts);		//Make the tree
		in.reset();
		out.writeBits(BITS_PER_INT, HUFF_TREE); //Huffman identifier 
		writeTree(root, out);					//Write Huffman tree to file
		String[] encodings = new String[ALPH_SIZE+1];
		makeEncodings(encodings,root, ""); //Generate Huffman encoding
		in.reset();
		while (true) 
		{
			int bits = in.readBits(BITS_PER_WORD);
			if (bits == -1) 
				break;
			String code = encodings[bits];					//Get Huffman encoding
			if (code != null) 
				out.writeBits(code.length(), Integer.parseInt(code, 2)); //Write the encoded bits
		}
		//Signals end of compressed file 
		String pseudo = encodings[PSEUDO_EOF]; 
		out.writeBits(pseudo.length(), Integer.parseInt(pseudo, 2)); 
		out.close();
	}

	/*
	 * Reads character frequences from input file
	 */
	private int[] getCounts(BitInputStream in) {
		int[] values = new int[ALPH_SIZE];
		while (true) {
			int chunk = in.readBits(BITS_PER_WORD); //Read next bit
			if (chunk == -1) 						//End of file
				break;
			else
				values[chunk] = values[chunk] + 1;		//Increment counter
		}
		return values; 
	}

	/* 
	 * Builds the Huffman tree
	 */
	private HuffNode makeTree(int[] values) {
		PriorityQueue<HuffNode> pq = new PriorityQueue<>();
		for(int i=0; i< values.length; i++) {
			if (values[i] > 0)
    			pq.add(new HuffNode(i,values[i],null,null)); //Leaf nodes
		}
		pq.add(new HuffNode(PSEUDO_EOF,1,null,null)); // account for PSEUDO_EOF having a single occurrence
		
		//Combine until only one node left
		while (pq.size() > 1) {
   			HuffNode left = pq.remove();
   			HuffNode right = pq.remove();
   			HuffNode t = new HuffNode(0, right.weight + left.weight, left, right);
   			pq.add(t);
		}
		HuffNode root = pq.remove();
		return root;
	}

	/*
	 * Recursively generate Huffman encodings for each character
	 */
	private void makeEncodings(String[] encoding, HuffNode node, String pathSoFar) {
		if (node == null) 
			return;
		if (node.left == null && node.right == null)
			encoding[node.value] = pathSoFar;				//store the binary code
		else {
			makeEncodings(encoding, node.left, pathSoFar + "0");
			makeEncodings(encoding, node.right, pathSoFar + "1");
		}
	}

	/* 
	 * Write the actual Huffman tree structure to the file
	 */
	private void writeTree(HuffNode node, BitOutputStream out) {
		if (node == null)
			return;
		if (node.right == null && node.left == null) {
			out.writeBits(1, 1);
			out.writeBits(BITS_PER_WORD + 1, node.value);
		}
		else {
			out.writeBits(1, 0);
			writeTree(node.left, out);
			writeTree(node.right, out);
		}
	}

	/**
	 * Decompresses a file. Output file must be identical bit-by-bit to the
	 * original.
	 *
	 * @param in
	 *            Buffered bit stream of the file to be decompressed.
	 * @param out
	 *            Buffered bit stream writing to the output file.
	 */
	public void decompress(BitInputStream in, BitOutputStream out){
		int bits = in.readBits(BITS_PER_INT);
		if (bits != HUFF_TREE)	
			throw new HuffException("invalid magic number "+ bits);
		HuffNode root = readTree(in);  //Reconstruct Huffman tree to decode
		HuffNode current = root;
		while (true) {
			bits = in.readBits(1);		//Only one bit read at a time
			if (bits == -1) {
				throw new HuffException("bad input, no PSEUDO_EOF");
			}
			else { 
				// use the zero/one value of the bit read
				// to traverse Huffman coding tree
				// if a leaf is reached, decode the character and print UNLESS
				// the character is pseudo-EOF, then decompression done
				if (bits == 0) 
					current = current.left; // read a 0, go left
				else                                   // read a 1, go right
					current = current.right;
				if (current.left == null && current.right == null) { // at leaf
					if (current.value == PSEUDO_EOF) 
						break;   // out of loop
					else {
						out.writeBits(BITS_PER_WORD, current.value);
						current = root; // start back after leaf
					}
				}
			}
		}
		out.close();
		}

		/* 
		 * Recursively reconstructs the Huffman tree 
		 */
		private HuffNode readTree(BitInputStream in) {
			int bit = in.readBits(1);
			if (bit == -1) 
				throw new HuffException("file ended so could not read bit");
			if (bit == 0) {
				HuffNode left = readTree(in);
				HuffNode right = readTree(in);
				return new HuffNode(0,0,left,right);
			}
			else {
				int value = in.readBits(BITS_PER_WORD +1);
				return new HuffNode(value,0,null,null);
			}
		}
			
}