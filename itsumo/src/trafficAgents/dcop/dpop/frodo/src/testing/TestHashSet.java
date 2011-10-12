/*
 * Created on Feb 15, 2006 by apetcu
 * Project Public
 * Package frodo.testing
 */
package frodo.testing;

import java.util.SortedMap;
import java.util.TreeMap;

import frodo.COP.ValuedNogood;
import frodo.CSP.ContextElement;
import frodo.CSP.VariableId;
import frodo.OCOP.MessageContext;
import frodo.OCOP.ODPOP.ValuedNogoodStorage;

public class TestHashSet {

	SortedMap elements;

	public TestHashSet() {
		elements = new TreeMap();
	}

	public void addElement(ContextElement cel){
		elements.put(cel.getVariableId(), cel);
	}

	public ContextElement getElement(ContextElement cel){
		return getElement(cel.getVariableId());
	}

	public ContextElement getElement(VariableId vid){
		return (ContextElement) elements.get(vid);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args){
		//		MessageContext mc = MessageContext.fromString("[7.0=0/8, 8.0=1/8]:9.0:9.0:9.0");
		ValuedNogood mc = ValuedNogood.fromString("[7.0=0/8, 8.0=1/8]:9.0:9.0:9.0");
		ValuedNogood mc2 = ValuedNogood.fromString("[9.0=0/8, 8.0=1/8]:9.0:9.0:9.0");
		System.out.println(mc.isCompatible(mc2));
		System.out.println(mc2.getContext().hashCode());
				
		ValuedNogoodStorage vngs = new ValuedNogoodStorage(0);
		System.out.println(mc.toString());
		System.out.println(mc2.toString());
		//		System.out.println(mc.isCompatible(mc2.getContext()));
		vngs.addNogood(mc);
		System.out.println(vngs.containsCompatibleNogood(mc2.getContext()));
		System.out.println(vngs.contains(mc2));
		vngs.addNogood(mc2);
		//		System.out.println(vngs.contains(mc));
		ValuedNogood mcc = ValuedNogood.fromString("[7.0=0/8, 9.0=0/8]:9.0:9.0:9.0");
		vngs.addNogood(mcc);
		System.out.println(vngs.toString());
		System.out.println(vngs.nogoodsSortedContext.values().toString());
		//		ContextElement cel = null;
		//		cel = ContextElement.buildFromString("8.0=*/8");
		//		System.out.println("Build cel=" + cel.toString());
		//		mc.addElement(cel);
		//		cel = ContextElement.buildFromString("4.0=1/8");
		//		System.out.println("Build cel=" + cel.toString());
		//		mc.addElement(cel);
		//		System.out.println("CTX=" + mc.toString());
		//
		//		MessageContext mc2 = MessageContext.fromString(mc.toString());
		//		System.out.println(mc2.toString());
		//		TestHashSet ths2 = new TestHashSet();
		//		ths2.addElement(ContextElement.buildFromString("10.0=2/8"));
		//		ths2.addElement(ContextElement.buildFromString("4.0=1/8"));
		//		System.out.println(ths2.elements.toString());
		//
		//		ths.elements.putAll(ths2.elements);
		//		System.out.println(ths.elements.toString());
		//		VariableId vid = new VariableId(4, 0);
		//		System.out.println(ths.getElement(vid));
		//		System.out.println(ths2.getElement(vid));
		//		System.out.println(ths.getElement(vid).equals(ths2.getElement(vid)));
		//		
		//		Integer unu = new Integer(1);
		//		Integer unu1 = new Integer(1);
		//		System.out.println(unu==unu1);
		//		System.out.println(unu.equals(unu1));
		//
	}

}
