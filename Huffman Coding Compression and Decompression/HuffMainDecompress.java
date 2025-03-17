/**
 * Main File for decompressing and restoring .hf files to their original format
 * User selects a .hf file, program outputs a file with .uhf extension
 * Recreates the Huffman tree from the given file to properly translate the binary back into readable characters 
 * Decompression statistics, such as bits read, bits written, and time taken are also displayed

 * @author Kavishka Bartlett
 */

import java.io.File;


public class HuffMainDecompress {
	private final static String UNHUFF_EXTENSION = ".uhf";

	//Generates default file name for decompressed file
	private static String getDefaultFileName(String name) {
		if (name.endsWith(".hf")) {									//If .hf, replace with .uhf
			return name.substring(0,name.length()-3)+UNHUFF_EXTENSION;
		}
		return name + UNHUFF_EXTENSION;				//Just add .uhf if not 
	}
	public static void main(String[] args) {
		System.out.println("Huffman Decompress Main");
		
		//User selects file to be decompressed
		System.out.println("Use FileDialog to choose file to decompress");
		File inf = FileSelector.selectFile("file to decompress");
		if (inf == null) {
			System.err.println("input file cancelled");
			return;
		}

		//User choose where to save original decompressed file
		System.out.println("Use FileDialog to choose file name/directory for decompressed file");
		String saveName = getDefaultFileName(inf.getName());
		File outf = FileSelector.saveFile("decompressed file name",saveName);
		if (outf == null) {
			System.err.println("output file cancelled");
			return;
		}

		//Initialize bit streams
		BitInputStream bis = new BitInputStream(inf);
		BitOutputStream bos = new BitOutputStream(outf);

		try {
			HuffProcessor hp = new HuffProcessor();
			
			//Start recording execution time
			long before = System.nanoTime();
			hp.decompress(bis, bos);
			long after = System.nanoTime();
			
			//Display performance metrics 
			System.out.printf("uncompress from %s to %s\n",
					inf.getName(), outf.getName());
			System.out.printf("file: %d bits to %d bits\n", inf.length() * 8, outf.length() * 8);
			System.out.printf("read %d bits, wrote %d bits\n",
					bis.bitsRead(), bos.bitsWritten());
			long diff = 8 * (outf.length() - inf.length());
			long diff2 = bos.bitsWritten() - bis.bitsRead();
			System.out.printf("%d compared to %d\n", diff, diff2);
			System.out.printf("Decompress took %d milliseconds", (after-before)/1000000);
		}
		catch (HuffException he) {
			boolean result = outf.delete();   //Delete file if an error occured 
			if (result) {
				System.err.printf("deleted file %s\n", outf.getName());
			}
			he.printStackTrace();
		}
	}
}