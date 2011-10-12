/*
 * Created on Aug 28, 2003
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
package frodo.utils;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.Random;
import java.util.StringTokenizer;
import java.util.Vector;

/**
 * @author apetcu
 * This is a class that offers a set of convenience methods for manipulating/printing vectors, arrays, etc
 */
public class HelperUtilities {
	//-----------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param arr
	 * @return the transpose of this matrix
	 */
	public static int[][] intArrayTranspose(int arr[][]) {
		int rez[][] = new int[arr[0].length][arr.length];
		for (int i = 0; i < arr[0].length; i++) {
			for (int j = 0; j < arr.length; j++) {
				rez[i][j] = arr[j][i];
			}
		}
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param v1
	 * @param v2
	 * @return the intersection of these two vectors
	 */
	public static Vector vectorIntersection(Vector v1, Vector v2) {
		Vector inters = new Vector();
		for (int i = 0; i < v1.size(); i++) {
			for (int j = 0; j < v2.size(); j++) {
				if (v1.elementAt(i).equals(v2.elementAt(j))) {
					int cnt = 0;
					boolean added = false;
					while (cnt < inters.size() && !added) {
						if (v1.elementAt(i).equals(inters.elementAt(cnt)))
							added = true;
						cnt++;
					}
					if (!added)
						inters.add(v1.elementAt(i));
				}
			}
		}
		return inters;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param v1
	 * @param v2
	 * @return true if v1 has any element in common with v2
	 */
	public static boolean vectorsIntersect(Vector v1, Vector v2) {
		if (vectorIntersection(v1, v2).size() > 0)
			return true;
		else
			return false;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	public static String vectorToString(Vector src) {
		String line = new String();
		if (src == null || src.size() == 0)
			return line;
		for (int i = 0; i < src.size() - 1; i++)
			line = line + src.elementAt(i) + " ";
		line = line + src.lastElement();

		return line;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	/**
	 * @param src
	 * @param delimiter
	 * @return a string composed of the elements of the <code>src</code>, separated by <code>delimiter</code> 
	 */
	public static synchronized String collectionToStringWithDelimiter(Collection src, String delimiter) {
		String line = new String();
		if (src == null || src.size() == 0)
			return line;
		for (Iterator iter = src.iterator(); iter.hasNext();) {
			line = line + iter.next();
			if (iter.hasNext())
				line = line + delimiter;
		}
		return line;
	}

	public static String vectorToStringWithTabs(Collection src) {
		return collectionToStringWithDelimiter(src, "\t");
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	public static Vector IntegerVectorFromString(String line) {
		Vector res = null;
		if (line != null) {
			try {
				res = new Vector();
				StringTokenizer st = new StringTokenizer(line);

				//reading integers
				while (st.hasMoreElements()) {
					Integer tmp = Integer.valueOf(st.nextToken(", \t\n\r\f"));
					res.add(tmp);
				}
			} catch (Exception e) {
				e.printStackTrace();
				return null;
			}
		}
		return res;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	//comma or space/tab separated
	public static int[] IntegerArrayFromString(String line) {
		int[] values = null;
		if (line != null) {
			String str = line.trim();
			if (str.startsWith("["))
				str = str.substring(1);
			if (str.endsWith("]"))
				str = str.substring(0, str.length() - 1);

			str = str.trim();
			if (!str.equalsIgnoreCase("")) {
				//				System.out.println("Gonna build context from:|" + str + "|");
				String[] parts = str.split("[\\s,]");
				values = new int[parts.length];
				for (int i = 0; i < parts.length; i++) {
					values[i] = Integer.valueOf(parts[i]).intValue();
				}
			}
		}
		return values;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	public static Vector DoubleVectorFromString(String line) {
		Vector res = null;
		if (line != null) {
			try {
				res = new Vector();
				StringTokenizer st = new StringTokenizer(line);

				//reading integers
				while (st.hasMoreElements()) {
					Double tmp = Double.valueOf(st.nextToken(", "));
					res.add(tmp);
				}
			} catch (Exception e) {
				e.printStackTrace();
				return null;
			}
		}
		return res;
	}

	//-----------------------------------------------------------------------------------------------------------------------//
	//compares the two vectors and returns the number of elements that differ
	public static int compareVectors(Vector x, Vector y) {
		int i = 0;
		if (x.size() != y.size())
			return Integer.MAX_VALUE;
		//		System.out.println("S1 = " + x.toString());
		//		System.out.println("S2 = " + y.toString());
		try {
			for (int j = 0; j < x.size(); j++) {
				if (!((Integer) x.elementAt(j)).equals((Integer) y.elementAt(j)))
					i++;
			}
		} catch (Exception e) {
			System.out.println("EXCEPTION:" + e.getMessage());
			e.printStackTrace();
			return Integer.MAX_VALUE;
		}
		//		System.out.println("=====>>>>> " + i);
		return i;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static int compareArrays(int[] x, int[] y) {
		int i = 0;
		if (x.length != y.length)
			return Integer.MAX_VALUE;
		//		System.out.println("S1 = " + x.toString());
		//		System.out.println("S2 = " + y.toString());
		for (int j = 0; j < x.length; j++) {
			if (x[j] != y[j])
				i++;
		}
		return i;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double arrayAverage(double[] array) {
		double rez = 0;
		for (int i = 0; i < array.length; i++) {
			rez += array[i];
		}
		rez = rez / array.length;
		return rez;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double arrayMinimum(double[] array) {
		double min = Double.POSITIVE_INFINITY;
		for (int i = 0; i < array.length; i++) {
			if (min > array[i])
				min = array[i];
		}
		return min;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double arrayMaximum(double[] array) {
		double max = Double.NEGATIVE_INFINITY;
		for (int i = 0; i < array.length; i++) {
			if (max < array[i])
				max = array[i];
		}
		return max;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//

	public static double vectorSum(Vector vect) {
		double rez = 0;
		for (int i = 0; i < vect.size(); i++) {
			if (vect.elementAt(0) instanceof Integer)
				rez += ((Integer) vect.elementAt(i)).intValue();
			else
				rez += ((Double) vect.elementAt(i)).doubleValue();
		}
		return rez;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//

	public static double vectorAverage(Vector vect) {
		if (vect.size() == 0)
			return 0;
		return vectorSum(vect) / vect.size();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double vectorMin(Vector vect) {
		double min = Double.MAX_VALUE;
		for (int i = 0; i < vect.size(); i++) {
			double cur = ((Double) vect.elementAt(i)).doubleValue();
			if (min > cur)
				min = cur;
		}
		return min;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double vectorMax(Vector vect) {
		double max = Double.MIN_VALUE;
		for (int i = 0; i < vect.size(); i++) {
			double cur = ((Double) vect.elementAt(i)).doubleValue();
			if (max < cur)
				max = cur;
		}
		return max;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double vectorVariance(Vector data) {
		double var = 0;
		double mean = vectorAverage(data);
		for (int i = 0; i < data.size(); i++) {
			double val = ((Long) data.get(i)).longValue();
			var = var + (val - mean) * (val - mean);
		}
		return Math.sqrt(var / (data.size() - 1));
	}

	//---------------------------------------------------------------------------------------//

	public static boolean isAllArrayTrue(boolean[] array) {
		for (int i = 0; i < array.length; i++) {
			if (!array[i])
				return false;
		}
		return true;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(boolean[] array) {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < array.length - 1; i++) {
			sb.append(array[i] + " ");
		}
		sb.append(array[array.length - 1]);
		return sb.toString();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(Object[] array) {
		return arrayToString(array, 0, array.length - 1);
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(Object[] array, int start, int end) {
		StringBuffer sb = new StringBuffer();
		if (end - start >= 0) {
			for (int i = start; i < end; i++) {
				sb.append(array[i] + " ");
			}
			sb.append(array[end]);
		}
		return sb.toString();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(int[] array, int start, int end) {
		if (array == null)
			return "null";
		StringBuffer sb = new StringBuffer();
		if (end - start >= 0) {
			for (int i = start; i < end; i++) {
				sb.append(array[i] + "\t");
			}
			sb.append(array[end]);
		}
		return sb.toString();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(double[] array) {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < array.length - 1; i++) {
			sb.append(array[i] + "\t");
		}
		sb.append(array[array.length - 1]);
		return sb.toString();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(int[] array) {
		return arrayToString(array, 0, array.length - 1);
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String arrayToString(int[][] array) {
		if (array == null)
			return "null";
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < array.length; i++) {
			sb.append(arrayToString(array[i]) + "\n");
		}
		return sb.toString();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static Vector addVectors(Vector v1, Vector v2) {
		if (v1.size() != v2.size()) {
			System.out.println("vector sizes differ..." + v1.size() + "!=" + v2.size());
			return null;
		}
		Vector rez = new Vector();
		for (int i = 0; i < v1.size(); i++) {
			Double o = new Double(((Double) v1.elementAt(i)).doubleValue() + ((Double) v2.elementAt(i)).doubleValue());
			rez.add(o);
		}
		return rez;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static int[] addArrays(int[] v1, int[] v2) {
		if (v1.length != v2.length) {
			System.out.println("vector sizes differ..." + v1.length + "!=" + v2.length);
			return null;
		}
		int[] rez = new int[v1.length];
		for (int i = 0; i < v1.length; i++) {
			rez[i] = v1[i] + v2[i];
		}
		return rez;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static Vector substractVectors(Vector v1, Vector v2) {
		if (v1 == null || v2 == null) {
			System.out.println("One vector is null: " + v1 + "==" + v2);
		}
		if (v1.size() != v2.size()) {
			System.out.println("vector sizes differ..." + v1.size() + "!=" + v2.size());
			return null;
		}
		Vector rez = new Vector();
		for (int i = 0; i < v1.size(); i++) {
			Double o = new Double(((Double) v1.elementAt(i)).doubleValue() - ((Double) v2.elementAt(i)).doubleValue());
			rez.add(o);
		}
		return rez;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static Vector divideVector(Vector v1, double div) {
		if (v1 == null) {
			System.out.println("cannot divide null vector");
			return null;
		}
		if (div == 0) {
			System.out.println("cannot divide by 0");
			return null;
		}

		for (int i = 0; i < v1.size(); i++) {
			v1.setElementAt(new Double(((Double) v1.elementAt(i)).doubleValue() / div), i);
		}
		return v1;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double[][] copyDoubleArray(double[][] src, double[][] dest) {
		if (src.length != dest.length || src[0].length != dest[0].length)
			return null;
		for (int i = 0; i < src.length; i++)
			for (int j = 0; j < src[0].length; j++)
				dest[i][j] = src[i][j];
		return src;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static Vector arrayToVector(double[] src) {
		if (src == null)
			return null;
		Vector dest = new Vector();
		for (int i = 0; i < src.length; i++)
			dest.add(new Double(src[i]));
		return dest;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static double[] vectorToArray(Vector src) {
		if (src == null)
			return null;
		double[] dest = new double[src.size()];
		for (int i = 0; i < dest.length; i++)
			dest[i] = ((Double) src.elementAt(i)).doubleValue();
		return dest;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static int[] generatePermutation(Random randomGenerator, int howm) {
		int[] perm = new int[howm];
		Vector orig = new Vector();
		for (int i = 0; i < howm; i++) {
			orig.add(new Integer(i));
		}
		for (int i = 0; i < howm; i++) {
			int which = randomGenerator.nextInt(howm - i);
			perm[i] = ((Integer) orig.elementAt(which)).intValue();
			orig.remove(which);
		}
		return perm;
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	public static String indent(String src, int level) {
		String[] parts = src.split("\n");
		StringBuffer rez = new StringBuffer();
		String indentul = new String();
		for (int i = 0; i < level; i++)
			indentul += "\t";
		for (int i = 0; i < parts.length; i++)
			rez.append(indentul + parts[i] + "\n");
		return rez.toString();
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public static int findInArray(Object[] arr, Object targ) {
		for (int i = 0; i < arr.length; i++) {
			//            System.out.print("Comparing " + arr[i] + " with " + targ);
			if (arr[i].equals(targ)) {
				//                System.out.println("-> " +i);
				return i;
			}
		}
		//        System.out.println("-> -1");
		return -1;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static int findInVector(Vector src, Object targ) {
		for (int i = 0; i < src.size(); i++) {
			//            System.out.print("Comparing " + arr[i] + " with " + targ);
			if (src.elementAt(i).equals(targ)) {
				//                System.out.println("-> " +i);
				return i;
			}
		}
		//        System.out.println("-> -1");
		return -1;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static boolean arrayContains(Object[] arr, Object targ) {
		System.out.print("Searching " + targ + " in " + arrayToString(arr));
		if (findInArray(arr, targ) >= 0) {
			System.out.println("->true");
			return true;
		} else {
			System.out.println("->false");
			return false;
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static Object[] union(Object[] unu, Object[] doi) {
		ArrayList arrl = new ArrayList();
		for (int i = 0; i < unu.length; i++) {
			if (!arrl.contains(unu[i]))
				arrl.add(unu[i]);
		}
		for (int i = 0; i < doi.length; i++) {
			if (!arrl.contains(doi[i]))
				arrl.add(doi[i]);
		}
		return arrl.toArray();
	}

	//	------------------------------------------------------------------//
	public static Object[] setMinus(Object[] unu, Object[] doi) {
		ArrayList arrl = new ArrayList();
		for (int i = 0; i < unu.length; i++) {
			if (!HelperUtilities.arrayContains(doi, unu[i]))
				arrl.add(unu[i]);
		}
		System.out.print("Set minus: " + arrayToString(unu) + " \\ " + arrayToString(doi));
		System.out.println("->" + arrayToString(arrl.toArray()));
		return arrl.toArray();
	}

	//	------------------------------------------------------------------//
	public static Object[] intersection(Object[] unu, Object[] doi) {
		ArrayList arrl = new ArrayList();
		for (int i = 0; i < unu.length; i++) {
			if (HelperUtilities.arrayContains(doi, unu[i]))
				arrl.add(unu[i]);
		}
		return arrl.toArray();
	}

	//-----------------------------------------------------------------------------------------------------------------//

	/**
	 * @param idTranslations
	 * @param agentID
	 * @return the first index where the specified int appears in this array, or -1 if not found
	 */
	public static int findInArray(int[] arr, int id) {
		for (int i = 0; i < arr.length; i++) {
			if (arr[i] == id)
				return i;
		}
		return -1;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static void copyArray(Object[] srcarray, Object[] destarray) {
		copyArrayOffset(srcarray, destarray, 0);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static void copyArrayOffset(Object[] srcarray, Object[] destarray, int offset) {
		if (srcarray.length + offset > destarray.length) {
			System.out.println("Arrays size mismatch: source has " + srcarray.length + " elements and destination has " + destarray.length + " elements.");
			System.exit(1);
		}
		for (int i = 0; i < srcarray.length; i++)
			destarray[i + offset] = srcarray[i];
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public static void copyArray(int[] srcarray, int[] destarray) {
		copyArrayOffset(srcarray, destarray, 0);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static void copyArrayOffset(int[] srcarray, int[] destarray, int offset) {
		if (srcarray.length + offset > destarray.length) {
			System.out.println("Arrays size mismatch: source has " + srcarray.length + " elements and destination has " + destarray.length + " elements.");
			System.exit(1);
		}
		for (int i = 0; i < srcarray.length; i++)
			destarray[i + offset] = srcarray[i];
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public static void main(String[] args) {
		Random rnd = new Random();
		for (int i = 0; i < 10; i++) {
			System.out.println(i + "\t" + HelperUtilities.arrayToString(HelperUtilities.generatePermutation(rnd, 4)));
		}
		System.out.println(HelperUtilities.indent("Gigi are mere\nsi mai are si niste pere\nsi alune", 3));
	}

}