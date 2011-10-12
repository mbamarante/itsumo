/*
 * Created on Jun 6, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP;

import java.io.Serializable;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Vector;

import frodo.CSP.ContextElement;
import frodo.CSP.VariableId;
import frodo.OCOP.DPOP.Dimension;

/**
 * @author apetcu
 * 
 */
public class UnsortedMessageContext implements Serializable, Cloneable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private Vector elements;

	//-----------------------------------------------------------------------------------------------------------------//
	public UnsortedMessageContext() {
		//		this.elements = new HashMap();
		elements = new Vector();
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public UnsortedMessageContext(UnsortedMessageContext altu) {
		elements = new Vector(altu.elements);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static UnsortedMessageContext fromString(String str) {
		UnsortedMessageContext mc = new UnsortedMessageContext();
		str = str.trim();
		if (str.startsWith("["))
			str = str.substring(1);
		if (str.endsWith("]"))
			str = str.substring(0, str.length() - 1);

		str = str.trim();
		//			System.out.println("Gonna build context from:|" + str + "|");
		String[] parts = str.split("\\s");
		for (int i = 0; i < parts.length; i++) {
			ContextElement cel = ContextElement.buildFromString(parts[i]);
			mc.addElement(cel);
		}
		return mc;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static UnsortedMessageContext fromDimensions(Dimension[] dims) {
		if (dims == null)
			return null;
		UnsortedMessageContext mc = new UnsortedMessageContext();
		for (int i = 0; i < dims.length; i++) {
			VariableId vid = new VariableId(dims[i].getId(), 0);
			ContextElement cel = new ContextElement(vid, new Integer(dims[i].getCurrentIndex()), dims[i].getSize());
			mc.addElement(cel);
		}
		return mc;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the elements.
	 */
	public Vector getElements() {
		return elements;
	}

	/**
	 * @param element
	 *            The element to add.
	 */
	public void addElement(ContextElement cel) {
		//		elements.put(cel.getVariableId(), cel);
		elements.add(cel);
	}

	public void addElementIfNotPresent(ContextElement cel) {
		if (!containsVariable(cel.getVariableId()))
			addElement(cel);
	}

	//	public ContextElement getElement(int i){
	//		return (ContextElement) elements.elementAt(i);
	//	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public boolean containsVariable(VariableId hay) {
		for (int i = 0; i < elements.size(); i++)
			if (((ContextElement) elements.elementAt(i)).getVariableId().equals(hay))
				return true;
		return false;
		//		return elements.containsKey(hay);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public String toString() {
		String rez = "[ ";

		for (Iterator iter = elements.iterator(); iter.hasNext();) {
			rez += iter.next().toString() + " ";
		}
		rez += "]";
		//		return elements.toString();
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public int size() {
		return elements.size();
	}

	public boolean isEmpty() {
		if (elements.size() == 0)
			return true;
		return false;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public void combineRegardlessValue(UnsortedMessageContext another) {
		for (Iterator iter = another.getElements().iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if (!this.containsVariable(cel.getVariableId()))
				addElement(cel);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * This joins the current context with the "another" one.
	 * They *must* be compatible
	 * @param another
	 */
	public void combine(UnsortedMessageContext another) {
		//		System.out.print("Combining " + toString() + "<>" + another.toString());
		if (another == null)
			return;
		//		elements.putAll(another.elements);
		for (int i = 0; i < another.elements.size(); i++) {
			ContextElement test = another.getElement(i);
			if (!this.containsVariable(test.getVariableId()))
				addElement(test);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public static UnsortedMessageContext combine(UnsortedMessageContext one, UnsortedMessageContext two) {
		UnsortedMessageContext newmc = new UnsortedMessageContext(one);
		newmc.combine(two);
		return newmc;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public HashMap getElementsWithValue() {
		HashMap otherContexts = new HashMap();
		for (Iterator iter = elements.iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if (cel.getValue() != null)
				otherContexts.put(cel.getVariableId(), cel);
		}

		return otherContexts;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public HashMap getElementsWithValue(Integer value) {
		HashMap otherContexts = new HashMap();
		for (Iterator iter = elements.iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if ((cel.getValue() == null && value == null) || (cel.getValue() != null && cel.getValue().equals(value)))
				otherContexts.put(cel.getVariableId(), cel);
		}
		return otherContexts;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public ContextElement getElement(int i) {
		return (ContextElement) elements.elementAt(i);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * @param another context
	 * @return the number of common context elements that have different value assignments
	 */
	private int clashes(UnsortedMessageContext another) {
		int rez = 0;
		for (Iterator iter = elements.iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if (cel.getValue() != null) {
				ContextElement theOtherOne = another.findContextElement(cel.getVariableId());
				if (theOtherOne != null)
					if (!cel.getValue().equals(theOtherOne.getValue()))
						rez++;
			}
		}
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static long possibleCombinations(UnsortedMessageContext test) {
		int rez = 1;
		for (Iterator iter = test.elements.iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			rez = rez * cel.getDomainSize();
		}
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//

	public boolean equals(Object other) {
		if (other instanceof UnsortedMessageContext) {
			UnsortedMessageContext test = (UnsortedMessageContext) other;
			if (elements.size() != test.size())
				return false;
			for (Iterator iter = elements.iterator(); iter.hasNext();) {
				ContextElement cel = (ContextElement) iter.next();
				ContextElement otherone = test.findContextElement(cel.getVariableId());
				if (otherone == null || !cel.getValue().equals(otherone.getValue()))
					return false;
				//				if (!test.containsVariable(cel.getVariableId()))
				//					return false;
			}
			return true;
		} else
			return false;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	private HashMap commonElements(UnsortedMessageContext other) {
		HashMap rez = new HashMap();
		//		int howm = 0;
		for (Iterator iter = elements.iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if (other.containsVariable(cel.getVariableId()))
				rez.put(cel.getVariableId(), cel);
		}
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	/*
	 private boolean disjoint(MessageContext other) {
	 if (this.commonElements(other).size() == 0)
	 return true;
	 else
	 return false;
	 }
	 */
	//-----------------------------------------------------------------------------------------------------------------//
	public boolean isCompatible(UnsortedMessageContext other) {
		if (clashes(other) > 0)
			return false;
		else
			return true;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	/*
	 public ContextElement findContextElement(ContextElement cel){
	 ContextElement test = (ContextElement) elements.get(cel.getVariableId());
	 if (test.equals(cel))
	 return test;
	 return null;
	 }
	 */
	//-----------------------------------------------------------------------------------------------------------------//
	private int findContextElementNDX(VariableId vid) {
		for (int i = 0; i < elements.size(); i++) {
			ContextElement cel = (ContextElement) elements.elementAt(i);
			if (cel.getVariableId().equals(vid))
				return i;
		}
		return -1;
	}

	public ContextElement findContextElement(VariableId vid) {
		int ndx = findContextElementNDX(vid);
		if (ndx > 0)
			return (ContextElement) elements.elementAt(ndx);
		return null;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public ContextElement removeContextElement(VariableId vid) {
		int ndx = findContextElementNDX(vid);
		if (ndx >= 0) {
			ContextElement rez = (ContextElement) elements.elementAt(ndx);
			elements.removeElementAt(ndx);
			return rez;
		}
		return null;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public Vector sort() {
		Collections.sort(this.elements);
		return this.elements;
	}

	//	-----------------------------------------------------------------------------------------------------------------//
	public Object clone() {
		UnsortedMessageContext mc = new UnsortedMessageContext();
		for (Iterator iter = elements.iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			ContextElement newCel = (ContextElement) cel.clone();
			mc.addElement(newCel);
		}
		return mc;
	}

	//-----------------------------------------------------------------------------------------------------------------//

	/**
	 * @param newCEL - the new assignment that I have to copy over mine
	 * @return true if target variable has been found, false otherwise
	 */
	private boolean updateElement(ContextElement newCEL) {
		ContextElement target = this.findContextElement(newCEL.getVariableId());
		if (target == null)
			return false;
		else {
			target.setValue(newCEL.getValue());
		}
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------//

	/**
	 * This updates the assignments of my variables to the assignments of the variables that are contained in the new context
	 * @param newCTX - the sample context
	 * @return - true if all elements from newCTX were found in my context, false otherwise
	 */
	public boolean update(UnsortedMessageContext newCTX) {
		boolean rez = true;
		for (Iterator iter = newCTX.getElements().iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if (this.updateElement(cel) == false)
				rez = false;
		}
		return rez;
	}

	/**
	 * Deletes all elements
	 */
	public void clear() {
		elements.clear();
	}

	/**
	 * @param vid - the id of the variable to find
	 * @return the assignment of this variable in this context, or null if variable not found
	 */
	public Integer getAssignment(VariableId vid) {
		ContextElement cel = findContextElement(vid);
		return cel.getValue();
	}
}
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//