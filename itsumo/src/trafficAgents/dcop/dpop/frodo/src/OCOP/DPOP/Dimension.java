/*
 * Created on Apr 2, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP.DPOP;

import java.util.ArrayList;

import frodo.utils.HelperUtilities;


/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Dimension {
	int id;

	int size;

	int currentIndex;

	/**
	 * 
	 */
	// I hope this does not break anything.. it means that at init, the index points nowhere
	public Dimension(int id, int size) {
		this(id, -1, size);
	}

	public Dimension(int id, int current_index, int size) {
		this.id = id;
		currentIndex = current_index;
		this.size = size;
	}

	public int getId() {
		return id;
	}

	public void setId(int name) {
		id = name;
	}

	public int getSize() {
		return size;
	}

	public void setSize(int size) {
		this.size = size;
	}

	public boolean equals(Object other) {
		if (other != null && other.getClass().equals(this.getClass())) {
			Dimension tocomp = (Dimension) other;
			//            System.out.println("XXXXXXXXXXX comparing XXXXXXXX " + id + " vs " + tocomp.getId());
			if (id == tocomp.getId())
				return true;
		}
		return false;
	}

	public String toString() {
		return new String(id + ":" + currentIndex + ":" + size);
	}

	public static Dimension fromString(String src) {
		src.trim();
		if (src.length() == 0)
			return null;
		String[] parts = src.split(":");
		if (parts.length == 3) {
			int id = Integer.parseInt(parts[0]);
			int ci = Integer.parseInt(parts[1]);
			int sz = Integer.parseInt(parts[2]);
			return new Dimension(id, ci, sz);
		} else if (parts.length == 2) {
			int id = Integer.parseInt(parts[0]);
			int sz = Integer.parseInt(parts[1]);
			return new Dimension(id, sz);
		} else
			return null;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	public static Dimension[] unionDimensions(Dimension[] unu, Dimension[] doi) {
		Object[] tmp = HelperUtilities.union(unu, doi);

		Dimension[] rez = new Dimension[tmp.length];
		for (int i = 0; i < tmp.length; i++)
			rez[i] = (Dimension) tmp[i];
		return rez;
	}

	//	------------------------------------------------------------------//
	public static Dimension findDimension(Dimension[] unu, int id) {
		for (int i = 0; i < unu.length; i++)
			if (unu[i].getId() == id)
				return unu[i];
		return null;
	}

	//	------------------------------------------------------------------//
	public static Dimension[] setMinusDimensions(Dimension[] unu, Dimension[] doi) {
		/*
		 Object[] tmp = HelperUtilities.setMinus(unu, doi);

		 Dimension[] rez = new Dimension[tmp.length];
		 for (int i = 0; i < tmp.length; i++)
		 rez[i] = (Dimension) tmp[i];
		 return rez;
		 */
		ArrayList arrl = new ArrayList();
		for (int i = 0; i < unu.length; i++) {
			boolean found = false;
			for (int j = 0; j < doi.length; j++) {
				if (unu[i].getId() == doi[j].getId())
					found = true;
			}
			if (!found) {
				arrl.add(unu[i]);
			}
		}
		Object tmp[] = arrl.toArray();
		Dimension[] rez = new Dimension[tmp.length];
		for (int i = 0; i < tmp.length; i++)
			rez[i] = (Dimension) tmp[i];
		return rez;
	}

	//	------------------------------------------------------------------//
	public static Dimension[] setMinusDimensions(Dimension[] unu, Dimension doi) {
		Dimension[] tmp = { doi };
		return Dimension.setMinusDimensions(unu, tmp);
	}

	//	------------------------------------------------------------------//
	public static Dimension[] intersectionDimensions(Dimension[] unu, Dimension[] doi) {
		/*
		 Object[] tmp = HelperUtilities.intersection(unu, doi);

		 Dimension[] rez = new Dimension[tmp.length];
		 for (int i = 0; i < tmp.length; i++)
		 rez[i] = (Dimension) tmp[i];
		 return rez;
		 */
		ArrayList arrl = new ArrayList();
		for (int i = 0; i < unu.length; i++) {
			boolean found = false;
			for (int j = 0; j < doi.length; j++) {
				if (unu[i].getId() == doi[j].getId())
					found = true;
			}
			if (found) {
				arrl.add(unu[i]);
			}
		}
		Object tmp[] = arrl.toArray();
		Dimension[] rez = new Dimension[tmp.length];
		for (int i = 0; i < tmp.length; i++)
			rez[i] = (Dimension) tmp[i];
		return rez;

	}

	//	------------------------------------------------------------------//

	public int getCurrentIndex() {
		return currentIndex;
	}

	public void setCurrentIndex(int currentIndex) {
		this.currentIndex = currentIndex;
	}

	//	------------------------------------------------------------------//
	public static int translateHyperToLinear(Dimension indexes[]) {
		/*
		 if (indexes.length != dimensions.length) {
		 //            return Integer.MIN_VALUE;
		 System.out.println("Index size does not match the message dimensions.");
		 System.out.println("Message dimensions:" + HelperUtilities.arrayToString(dimensions));
		 System.out.println("Index dimensions:" + HelperUtilities.arrayToString(indexes));
		 System.exit(1);
		 }
		 */
		//start with the last offset
		int ndx = indexes[indexes.length - 1].getCurrentIndex();
		int prod = 1;
		for (int i = indexes.length - 1; i >= 1; i--) {
			prod *= indexes[i].getSize();
			ndx = ndx + prod * indexes[i - 1].getCurrentIndex();
		}
		return ndx;
	}

	//	------------------------------------------------------------------//
	public static Dimension[] translateLinearToHyper(Dimension[] dims, int index) {
		//		int[] rez = new int[dimensions.length];
		if (dims == null) {
			return null;
		} else {
			int howm = index;
			for (int i = dims.length - 1; i >= 0; i--) {
				dims[i].setCurrentIndex(howm % dims[i].getSize());
				howm = howm / dims[i].getSize();
			}
			return dims;
		}
	}

	//	------------------------------------------------------------------//
	public static int getTotalSize(Dimension[] dimensions) {
		int prod = 1;
		for (int i = 0; i < dimensions.length; i++) {
			prod *= dimensions[i].getSize();
		}
		return prod;
	}

	//	------------------------------------------------------------------//
	public static void setIndexes(Dimension[] srcdims, Dimension[] targdims) {
		for (int i = 0; i < srcdims.length; i++) {
			Dimension srccrt = srcdims[i];
			int where = HelperUtilities.findInArray(targdims, srccrt);
			if (where > -1)
				targdims[where].setCurrentIndex(srccrt.getCurrentIndex());
		}

	}

	//	------------------------------------------------------------------//
	public int hashCode() {
		return id;
	}
	//	------------------------------------------------------------------//
}
