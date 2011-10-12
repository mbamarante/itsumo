package frodo.OCOP.communication;

import java.util.Vector;

import frodo.CSP.ContextElement;
import frodo.OCOP.MessageContext;

public class TopoProbe extends MessageContext {

	private static final long serialVersionUID = 1L;

	Vector elementsAsVector;//this is just for using in the context of the DFS building, where the order of the elements is important.

	//	private Vector elementsList;//this is the same set of elements, but stored like this for sequential/random access
	public TopoProbe() {
		super();
		elementsAsVector = new Vector();
	}

	public void addElement(ContextElement cel) {
		super.addElement(cel);
		elementsAsVector.add(cel);
	}
	public String toString() {
		return elementsAsVector.toString();
	}
	public ContextElement getElement(int i) {
		return (ContextElement) elementsAsVector.elementAt(i);
	}
	public static TopoProbe buildFromString(String str) {
		TopoProbe mc = new TopoProbe();
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
}
