/*
 * CSPAbstractVariable.java
 *
 * Created on October 18, 2002, 3:44 PM
 */

package frodo.dbreakout.CSP;

import java.util.Vector;

import frodo.CSP.VariableId;


/**
 *
 * @author  apetcu
 */
public class CSPVariableDBA extends frodo.CSP.CSPVariable {

	Vector domainCounters;
	Vector NIDomain;
	//-----------------------------------------------------------------------//
	public CSPVariableDBA(VariableId vid, Integer myv) {
		super(vid, myv);
	}
	//-----------------------------------------------------------------------//
	public CSPVariableDBA(VariableId vid, Vector domain, Vector constr) {
		super(vid, domain, constr);
		if (domain != null) { //setting up the initial counters to 0;
			this.domainCounters = new Vector();
			for (int i = 0; i < domain.size(); i++) {
				domainCounters.add(new Integer(0));
			}
		}
	}

	/**
	 * 
	 * @uml.property name="domainCounters"
	 */
	//-----------------------------------------------------------------------//
	public Vector getDomainCounters() {
		return domainCounters;
	}

	/**
	 * 
	 * @uml.property name="nIDomain"
	 */
	//-----------------------------------------------------------------------//
	public Vector getNIDomain() {
		return NIDomain;
	}

	/**
	 * 
	 * @uml.property name="nIDomain"
	 */
	public void setNIDomain(Vector dom) {
		NIDomain = dom;
	}

	//-----------------------------------------------------------------------//
	public void updateDomainCounters(Vector extDom) {
		if (extDom != null) {
			for (int i = 0; i < extDom.size(); i++) {
				for (int j = 0; j < domain.size(); j++) {
					if (extDom.elementAt(i).equals(domain.elementAt(j))) {
						Integer curCnt = (Integer) domainCounters.elementAt(j);
						domainCounters.setElementAt(new Integer(curCnt.intValue() + 1), j);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------//
	public void sortDomain_Counters() {
		for (int i = domain.size(); --i >= 0;) {
			boolean flipped = false;
			for (int j = 0; j < i; j++) {
				Integer unu = (Integer) domainCounters.elementAt(j);
				Integer doi = (Integer) domainCounters.elementAt(j + 1);
				if (unu.intValue() > doi.intValue()) {

					Object dom_unu = domain.elementAt(j);
					Object dom_doi = domain.elementAt(j + 1);
					domain.setElementAt(dom_doi, j);
					domain.setElementAt(dom_unu, j + 1);

					domainCounters.setElementAt(doi, j);
					domainCounters.setElementAt(unu, j + 1);

					flipped = true;
				}
			}
			if (!flipped) {
				return;
			}
		}
	}
	//-----------------------------------------------------------------------//
}