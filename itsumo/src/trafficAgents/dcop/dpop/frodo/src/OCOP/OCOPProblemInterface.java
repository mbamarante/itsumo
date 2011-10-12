/*
 * Created on Jun 7, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP;

import java.io.Serializable;
import java.util.Vector;

import frodo.CSP.BinaryConstraint;
import frodo.CSP.CSPVariable;
import frodo.utils.Logger;


/**
 * @author apetcu
 * 
 * TODO To change the template for this generated type comment go to Window - Preferences - Java - Code Style - Code Templates
 */

public interface OCOPProblemInterface extends Serializable {

	/**
	 * 
	 * @uml.property name="variables"
	 */
	//--------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------//
	public abstract void setVariables(Vector v);

	/**
	 * 
	 * @uml.property name="variables"
	 */
	public abstract Vector getVariables();

	public abstract void addVariable(Object v);

	public abstract boolean deleteVariable(Object v);

	public abstract int getNumberOfVariables();

	public abstract CSPVariable getVariable(int i);

	/**
	 * 
	 * @uml.property name="nodes"
	 */
	//this is just for convenience, to return the first variable; this is good in case there is actually
	//--------------------------------------------------------------------------------------------------//
	public abstract void setNodes(Vector nodes);

	/**
	 * 
	 * @uml.property name="nodes"
	 */
	public abstract Vector getNodes();

	public abstract void addNode(Object n);

	public abstract boolean deleteNode(Object n);

	public abstract int getNumberOfNodes();

	/**
	 * Method getNeighbour. returns the neighbor with the specified id, or null if not found. Note that this is not the i-th neighbor!!!!
	 * 
	 * @param id
	 * @return OCOPAbstractNode
	 */
	public abstract OCOPAbstractNode getNodeById(int id);

	public abstract OCOPAbstractNode getNodeByIndex(int index);

	/**
	 * 
	 * @uml.property name="constraints"
	 */
	//------------------------------------------------------------------------------//
	public abstract void setConstraints(Vector constraints);

	/**
	 * 
	 * @uml.property name="constraints"
	 */
	public abstract Vector getConstraints();

	public abstract void addConstraint(Object bc);

	public abstract boolean deleteConstraint(Object bc);

	public abstract int getNumberOfConstraints();

	public abstract BinaryConstraint getConstraint(int i);

	//--------------------------------------------------------------------------------------------------//
	public abstract String toStringSerialize();

	/**
	 * 
	 * @uml.property name="logger"
	 * @uml.associationEnd 
	 * @uml.property name="logger" multiplicity="(0 1)"
	 */
	//--------------------------------------------------------------------------------------------------//
	public abstract Logger getLogger();

	/**
	 * 
	 * @uml.property name="logger"
	 */
	public abstract void setLogger(Logger myLogger);

}