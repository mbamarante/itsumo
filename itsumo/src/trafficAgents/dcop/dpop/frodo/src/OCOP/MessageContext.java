/*
 * Created on Jun 6, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP;

import java.io.Serializable;
import java.util.Iterator;
import java.util.SortedMap;
import java.util.TreeMap;

import frodo.CSP.ContextElement;
import frodo.CSP.VariableId;
import frodo.OCOP.DPOP.Dimension;

/**
 * @author apetcu
 * 
 */
public class MessageContext implements Serializable, Cloneable, Comparable {

	private static final long serialVersionUID = 1L;

	SortedMap elements;

	//I just update this when adding a new element to the set.
	//I use it only in getElements(int index);

	//-----------------------------------------------------------------------------------------------------------------//
	public MessageContext() {
		elements = new TreeMap();
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public static MessageContext fromString(String str){
		MessageContext mc = new MessageContext();
		str = str.trim();
		if (str.startsWith("["))
			str = str.substring(1);
		if (str.endsWith("]"))
			str = str.substring(0, str.length() - 1);

		str = str.trim();
		//System.out.println("Gonna build context from:|" + str + "|");
		String[] parts = str.split(",");
		for (int i = 0; i < parts.length; i++) {
			ContextElement cel = ContextElement.buildFromString(parts[i]);
			mc.addElement(cel);
		}
		return mc;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static MessageContext fromDimensions(Dimension[] dims){
		if (dims == null)
			return null;
		MessageContext mc = new MessageContext();
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
	public SortedMap getElements(){
		return elements;
	}

	/**
	 * @param element
	 *            The element to add.
	 */
	public void addElement(ContextElement cel){
		elements.put(cel.getVariableId(), cel);
	}

	public void addElementIfNotPresent(ContextElement cel){
		if (!containsVariable(cel.getVariableId()))
			addElement(cel);
	}

	//	public ContextElement getElement(int i){
	//		return (ContextElement) elements.elementAt(i);
	//	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public boolean containsVariable(VariableId hay){
		return elements.containsKey(hay);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public String toString(){
		return elements.values().toString();
		//		String rez = "[ ";
		//
		//		for (Iterator iter = elements.iterator(); iter.hasNext();) {
		//			rez += iter.next().toString() + " ";
		//		}
		//		rez += "]";
		//		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public int size(){
		return elements.size();
	}

	public boolean isEmpty(){
		if (elements.size() == 0)
			return true;
		return false;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * This joins the current context with the "another" one.
	 * They *must* be compatible
	 * @param another
	 */
	public void combine(MessageContext another){
		//		System.out.print("Combining " + toString() + "<>" + another.toString());
		if (another != null)
			elements.putAll(another.elements);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public static MessageContext combine(MessageContext one, MessageContext two){
		MessageContext newmc = new MessageContext();
		newmc.elements.putAll(one.elements);
		newmc.elements.putAll(two.elements);
		;
		return newmc;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * @param another context
	 * @return the number of common context elements that have different value assignments
	 */
	private int clashes(MessageContext another){
		int rez = 0;
		for (Iterator iter = elements.values().iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			ContextElement theOtherOne = another.getContextElement(cel.getVariableId());
			if (theOtherOne != null) {//I found my counterpart
				if (!cel.equals(theOtherOne))//it does not match my value
					rez++;
			}
		}
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static long possibleCombinations(MessageContext test){
		int rez = 1;
		for (Iterator iter = test.elements.values().iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			rez = rez * cel.getDomainSize();
		}
		return rez;
	}

	//-----------------------------------------------------------------------------------------------------------------//

	public boolean equals(Object other){
		MessageContext test = (MessageContext) other;
				if (elements.size() != test.size())
					return false;
				for (Iterator iter = elements.values().iterator(); iter.hasNext();) {
					ContextElement cel = (ContextElement) iter.next();
					ContextElement otherone = test.getContextElement(cel.getVariableId());
					if (!cel.equals(otherone))
						return false;
				}
				return true;
//		return hashCode() - test.hashCode() == 0;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public SortedMap commonElements(MessageContext other){
		SortedMap rez = new TreeMap();
		//		int howm = 0;
		for (Iterator iter = elements.values().iterator(); iter.hasNext();) {
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
	public boolean isCompatible(MessageContext other){
		for (Iterator iter = elements.values().iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			ContextElement theOtherOne = other.getContextElement(cel.getVariableId());
			if (theOtherOne != null && !cel.equals(theOtherOne))//it does not match my value
				return false;
		}
		return true;
		//		if (clashes(other) > 0)
		//			return false;
		//		else
		//			return true;
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
	public ContextElement getContextElement(VariableId vid){
		return (ContextElement) elements.get(vid);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public ContextElement removeContextElement(VariableId vid){
		return (ContextElement) elements.remove(vid);
	}

	//-----------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------//
	public Object clone(){
		MessageContext mc = new MessageContext();
		for (Iterator iter = elements.values().iterator(); iter.hasNext();) {
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
	private boolean updateElement(ContextElement newCEL){
		ContextElement target = this.getContextElement(newCEL.getVariableId());
		if (target == null)
			return false;
		else
			target.setValue(newCEL.getValue());
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------//

	/**
	 * This updates the assignments of my variables to the assignments of the variables that are contained in the new context
	 * @param newCTX - the sample context
	 * @return - true if all elements from newCTX were found in my context, false otherwise
	 */
	public boolean update(MessageContext newCTX){
		boolean rez = true;
		for (Iterator iter = newCTX.getElements().values().iterator(); iter.hasNext();) {
			ContextElement cel = (ContextElement) iter.next();
			if (this.updateElement(cel) == false)
				rez = false;
		}
		return rez;
	}

	/**
	 * Deletes all elements
	 */
	public void clear(){
		elements.clear();
	}

	public int hashCode(){
		/*		
		 int code = 0;
		 for (Iterator iterator = elements.values().iterator(); iterator.hasNext();) {
		 ContextElement element = (ContextElement) iterator.next();
		 code = code * ContextElement.MAX_VALUES_PER_VARIABLE + element.hashCode();
		 System.out.println("Code=" + code);
		 }
		 return code;
		 */
		return elements.hashCode();
	}

	/**
	 * 
	 * @throws ClassCastException
	 */
	public int compareTo(Object ref){
		MessageContext new_name = (MessageContext) ref;
		Iterator iter2 = new_name.elements.values().iterator();
		for (Iterator iter1 = elements.values().iterator(); iter1.hasNext();) {
			ContextElement element1 = (ContextElement) iter1.next();
			ContextElement element2 = (ContextElement) iter2.next();
			int tmpcmp = element1.compareTo(element2);
			if (tmpcmp != 0)
				return tmpcmp;
		}
		return 0;
		//		return hashCode() - new_name.hashCode();
	}
}
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//