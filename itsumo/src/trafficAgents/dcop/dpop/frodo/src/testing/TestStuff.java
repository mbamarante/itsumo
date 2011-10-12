/*
 * Created on Dec 1, 2005 by apetcu
 * Project Public
 * Package testbed.testing
 */
package frodo.testing;

import frodo.COP.ValuedConstraint;
import frodo.COP.ValuedNogood;
import frodo.OCOP.ODPOP.GoodFilter;
import frodo.OCOP.ODPOP.ValuedNogoodStorage;
import frodo.utils.Logger;

public class TestStuff {

	/**
	 * @param args
	 */
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		ValuedNogood t1 = ValuedNogood.fromString("[8.0=0/4, 9.0=1/4, 10.0=3/4]:12.0");
		ValuedNogood t2 = ValuedNogood.fromString("[8.0=3/4, 9.0=1/4, 10.0=0/4]:16.0");
		System.err.println("EGALE: " + t1.equals(t2));
		
		//		
		//		MessageContext mc = new MessageContext();
		//		System.out.println(mc.toString());
		//		ContextElement cel = null;
		//		cel = ContextElement.buildFromString("8.0=*/8");
		//		System.out.println("Build cel=" + cel.toString());
		//		mc.addElement(cel);
		//		cel = ContextElement.buildFromString("4.0=1/8");
		//		System.out.println("Build cel=" + cel.toString());
		//		mc.addElement(cel);
		//		System.out.println("CTX=" + mc.toString());
		//
		//		
		//		ValuedNogood vng = new ValuedNogood(mc, -1);
		//		System.out.println("VNG=" + vng.toString());0
		//
		//		ValuedNogood nog1 = ValuedNogood.fromString("[8.0=1/8, 5.0=1/8]:0.0");
		//		System.out.println(nog1.toString());
		//		ValuedNogood nog2 = ValuedNogood.fromString("[ 8.0=1/8, 5.0=0/8 ]:0.0");
		//		System.out.println(nog2.toString());
		//		System.out.println(nog1.equals(nog2));
		ValuedNogood nog1 = ValuedNogood.fromString("[2.0=0/8, 19.0=0/8, 6.0=0/8]:0.0");
		ValuedNogood nog2 = ValuedNogood.fromString("[2.0=0/8, 19.0=0/8]:0.0");
		ValuedConstraint vc1 = ValuedConstraint.fromString("0	10.0@-@8.0	0,0:1	0,1:3	0,2:0	0,3:8	1,0:1	1,1:7	1,2:4	1,3:6	2,0:7	2,1:2	2,2:3	2,3:4	3,0:3	3,1:5	3,2:6	3,3:8");
		ValuedConstraint vc2 = ValuedConstraint.fromString("0	10.0@-@9.0	0,0:6	0,1:8	0,2:4	0,3:5	1,0:5	1,1:6	1,2:9	1,3:5	2,0:4	2,1:1	2,2:7	2,3:8	3,0:5	3,1:9	3,2:8	3,3:3");
		System.out.println("VC1:" + vc1.toString());
		System.out.println("VC2:" + vc2.toString());

		ValuedNogoodStorage sto1 = ValuedNogoodStorage.fromRelation(vc1);
		ValuedNogoodStorage sto2 = ValuedNogoodStorage.fromRelation(vc2);

		ValuedNogoodStorage sto11 = new ValuedNogoodStorage(1);
		ValuedNogoodStorage sto21 = new ValuedNogoodStorage(1);
		Logger lgo = new Logger("tests.txt");
		lgo.setToSysOut(true);
		GoodFilter gf = new GoodFilter(sto11, sto21, null, lgo);
		sto11.setFilter(gf);
		sto21.setFilter(gf);
		boolean done = false;
		for (int i = 0; i < sto1.size(); i++) {
			ValuedNogood vng1 = sto1.getNogood(i);
			System.out.println("ADDING " + vng1 + " to Filter");
			sto11.addNogood(vng1);
			gf.filterNewNogood(sto11);
			System.out.println("ADDED " + vng1 + " to Filter:\n\t" + gf.toStringRecursive(0));
		}
		System.out.println("-----------------------------------------");
		System.out.println("-----------------------------------------");
		for (int i = 0; i < sto2.size(); i++) {
			System.out.println("-----------------------------------------");
			ValuedNogood vng2 = sto2.getNogood(i);
			System.out.println("ADDING " + vng2 + " to Filter");
			sto21.addNogood(vng2);
			if (i == sto2.size() - 1) {
				System.out.println("Final insert: " + vng2);
			}
			gf.filterNewNogood(sto21);
			System.out.println("ADDED " + vng2 + " to Filter:\n\t" + gf.toStringRecursive(0));
			//			System.out.println("CHECKING:");
			//			int cite = 0;
			//			for(int j=0;j<sto11.size();j++) {
			//				ValuedNogood test = sto11.getNogood(j);
			//				System.out.println("\tTEST " + vng2 + " vs " + test + " -> " + test.isCompatible(vng2));
			//				if(test.isCompatible(vng2))
			//					cite++;
			//			}
			//			System.out.println("CHECKING gives " +cite + " compatibles.");
		}
		for (int i = 0; i < gf.total.size(); i++) {
			ValuedNogood vng2 = gf.total.getNogood(i);
			System.out.println(i + " Checking " + vng2 + "\tfor optimality ->" + gf.proveOptimal(vng2));
		}
		//		
		//		while (!done) {
		//			ValuedNogood vng1 = sto1.more();
		//			ValuedNogood vng2 = sto2.more();
		//			if (vng1 == null || vng2 == null) {
		//				done = true;
		//			} else {
		//				System.out.println("ADDING " + vng1 + " to Filter");
		//				sto11.addNogood(vng1);
		//				gf.filterNewNogood(sto11);
		//				System.out.println("ADDED " + vng1 + " to Filter:\n\t" + gf.toStringRecursive(0));
		//				System.out.println("ADDING " + vng2 + " to Filter");
		//				sto21.addNogood(vng2);
		//				gf.filterNewNogood(sto21);
		//				System.out.println("ADDED " + vng2 + " to Filter:\n\t" + gf.toStringRecursive(0));
		//			}
		//		}
		//		//		for (Iterator iter = sto1.ge.iterator(); iter.hasNext();) {
		//		//			type element = (type) iter.next();
		//		//		}
		//		System.out.println("NOG1=" + nog1);
		//		System.out.println("NOG2=" + nog2);
		//		System.out.println("NOG1=NOG2" + nog1.isCompatible(nog2));
	}
}
