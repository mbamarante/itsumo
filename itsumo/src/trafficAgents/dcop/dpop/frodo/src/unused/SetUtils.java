/*
 * Created on Jun 10, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.unused;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Vector;

import frodo.CSP.VariableId;


/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class SetUtils {

	/**
	 * 
	 * @uml.property name="sets"
	 * @uml.associationEnd 
	 * @uml.property name="sets" multiplicity="(0 -1)" elementType="testbed.CSP.VariableId"
	 */
	Vector sets = new Vector();

	//-------------------------------------------------------------------------------------------------------//
	public SetUtils() {
		sets = new Vector();
	}

	//-------------------------------------------------------------------------------------------------------//
	public int findIdInVectorOfSets(VariableId id) {
		int rez = -1;
		int i = 0;
		while (rez == -1 && i < sets.size()) {
			HashSet hs = (HashSet) sets.elementAt(i);
			if (hs.contains(id))
				rez = i;
			i++;
		}
		return rez;
	}

	//-------------------------------------------------------------------------------------------------------//
	public boolean moveIdInVectorOfSets(VariableId id, int newpos) {
		System.out.println("Gonna move " + id.toString() + " to " + newpos);
		//	System.out.println("\nBEFORE: \n" + toString());
		int oldpos = findIdInVectorOfSets(id);
		//		System.out.println("\nfound old at" + oldpos);
		if (oldpos != -1) {
			HashSet hsold = (HashSet) sets.elementAt(oldpos);
			hsold.remove(id);
			if (hsold.isEmpty())
				sets.remove(hsold);
			HashSet hsnew = (HashSet) sets.elementAt(newpos);
			hsnew.add(id);
		}
		//		System.out.println("\nAFTER: \n" + toString());
		return false;
	}

	//-------------------------------------------------------------------------------------------------------//
	public boolean collapseSets(int id1, int id2) {
		//		System.out.println("Gonna collapse " + id1 + " to " + id2);
		//		System.out.println("\nBEFORE: \n" + toString());
		HashSet hsold = (HashSet) sets.elementAt(id1);
		HashSet hsnew = (HashSet) sets.elementAt(id2);
		Iterator it = hsold.iterator();
		while (it.hasNext()) {
			VariableId vid = (VariableId) it.next();
			hsnew.add(vid);
		}
		sets.remove(hsold);
		//		System.out.println("\nAFTER: \n" + toString());
		return false;
	}

	//-------------------------------------------------------------------------------------------------------//
	public boolean add(Object o) {
		return sets.add(o);
	}

	//-------------------------------------------------------------------------------------------------------//
	public String toString() {
		String r = new String();
		for (int i = 0; i < sets.size(); i++) {
			r = r + i + "\t" + sets.elementAt(i).toString() + "\n";
		}
		return r;
	}
	//-------------------------------------------------------------------------------------------------------//
}