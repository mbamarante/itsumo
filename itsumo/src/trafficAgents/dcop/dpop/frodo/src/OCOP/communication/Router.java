/*
 * Created on Apr 27, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP.communication;

import frodo.OCOP.MessageContext;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.graph.NeighborGroup;

/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Router {
	OCOPAbstractNode responsible;

	NeighborGroup targets;

	Integer idResponsible;

	/**
	 * @param responsible
	 * @param targets
	 */
	public Router(OCOPAbstractNode responsible, NeighborGroup targets) {
		super();
		this.responsible = responsible;
		this.targets = targets;
		this.idResponsible = new Integer(responsible.getId());
	}

	public Router(OCOPAbstractNode responsible) {
		this(responsible, new NeighborGroup(new MessageContext()));
	}

	public OCOPAbstractNode getResponsible() {
		return responsible;
	}

	public void setResponsible(OCOPAbstractNode responsible) {
		this.responsible = responsible;
	}

	public NeighborGroup getTargets() {
		return targets;
	}

	public void setTargets(NeighborGroup targets) {
		this.targets = targets;
	}

	public boolean containsTarget(int id) {
		return targets.containsNeighbor(id);
	}

	public Integer getIdResponsible() {
		return idResponsible;
	}

	public void setIdResponsible(Integer idResponsible) {
		this.idResponsible = idResponsible;
	}
}
