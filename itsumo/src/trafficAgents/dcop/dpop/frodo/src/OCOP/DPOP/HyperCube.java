/*
 * Created on Apr 2, 2005
 */
package frodo.OCOP.DPOP;

import java.util.Vector;

import frodo.COP.StabilityConstraint;
import frodo.COP.ValuedConstraint;
import frodo.CSP.VariableId;
import frodo.OCOP.MessageContext;
import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 */
//------------------------------------------------------------------//
public class HyperCube {
	//------------------------------------------------------------------//
	private int values[];

	private Dimension[] dimensions;

	public Dimension boundDimension = new Dimension(-1, 0, 2);

	private MessageContext context;

	//	------------------------------------------------------------------//
	public HyperCube(Dimension[] dims, int[] vals) {
		this.dimensions = dims;
		values = vals;
	}

	//	------------------------------------------------------------------//
	public HyperCube(Dimension[] dims) {
		this(dims, null);
		int prod = 1;
		for (int i = 0; i < dims.length; i++) {
			prod *= dims[i].getSize();
		}
		values = new int[prod];
	}

	//------------------------------------------------------------------//
	//	public HyperCube() {
	//		this.dimensions = new Dimensions[];
	//	}
	//------------------------------------------------------------------//
	public HyperCube(ValuedConstraint vc) {
		Dimension dim1 = new Dimension(vc.getSourceId().getAgentID(), vc.getFrom().getDomainSize());
		Dimension dim2 = new Dimension(vc.getDestinationId().getAgentID(), vc.getTo().getDomainSize());
		if (vc.getArity() == 2) {
			//this is a binary constraint, thus the hc is a table
			this.dimensions = new Dimension[2];
			dimensions[0] = dim1;
			dimensions[1] = dim2;
		} else if (vc.getArity() == 1) {
			//this is a unary constraint, thus the hc is a vector
			this.dimensions = new Dimension[1];
			dimensions[0] = dim1;
		}
		int prod = 1;
		for (int i = 0; i < dimensions.length; i++) {
			prod *= dimensions[i].getSize();
		}
		values = new int[prod];
		int[][] origvals = vc.getValuations();
		for (int i = 0; i < origvals.length; i++) {
			for (int j = 0; j < origvals[0].length; j++) {
				values[i * origvals[0].length + j] = origvals[i][j];
			}
		}
	}

	//------------------------------------------------------------------//
	public static HyperCube fromRelation(ValuedConstraint vc){
		HyperCube rez = new HyperCube(vc);
		return rez;
	}

	//------------------------------------------------------------------//
	public int getValue(Dimension indexes[]){
		return values[Dimension.translateHyperToLinear(indexes)];
	}

	public int getValue(int i){
		return values[i];
	}

	//	------------------------------------------------------------------//
	public void setValue(Dimension indexes[], int val){
		values[Dimension.translateHyperToLinear(indexes)] = val;
	}

	public void setValue(int i, int val){
		values[i] = val;
	}

	//	------------------------------------------------------------------//
	public Dimension[] getDimensions(){
		return this.dimensions;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	public HyperCube join(HyperCube other){
		// UTIL + 0 = UTIL 
		if (other == null)
			return this;
		//find the union of the two dimension sets
		Dimension[] unionDim = Dimension.unionDimensions(this.getDimensions(), other.getDimensions());
		//System.out.println("Union dimensions: " + HelperUtilities.arrayToString(unionDim));

		//build the new HC 
		HyperCube join = new HyperCube(unionDim);
		//making a copy of the union dimensions, relevant for the first message (this one)
		Dimension[] firstDims = this.getDimensions();
		//making a copy of the union dimensions, relevant for the second message (other)
		Dimension[] secondDims = other.getDimensions();
		//these will be the indexes needed to access its elements
		for (int i = 0; i < join.getTotalSize(); i++) {
			Dimension.translateLinearToHyper(unionDim, i);
			Dimension.setIndexes(unionDim, firstDims);
			Dimension.setIndexes(unionDim, secondDims);
			//            System.out.println("Element " + HelperUtilities.arrayToString(unionDim));
			//compute every element of the new HC
			int firstVal = this.getValue(firstDims);
			int secondVal = other.getValue(secondDims);
			join.setValue(unionDim, firstVal + secondVal);
		}
		return join;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	public HyperCube stabilityJoin(StabilityConstraint stabc, int curIndex){
		Dimension stabDim = Dimension.findDimension(getDimensions(), stabc.getSourceId().getAgentID());
		if (stabDim == null)
			return null;
		for (int i = 0; i < values.length; i++) {
			Dimension.translateLinearToHyper(dimensions, i);
			values[i] = values[i] + stabc.getValuations()[curIndex][stabDim.getCurrentIndex()];
		}
		return this;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	/**
	 * This is the optimal projection operation. It eliminates the specified set of dimensions out of this hypercube
	 * @param projectOuts
	 * @return the projection that eliminates (optimizes) projectOuts 
	 */
	public HyperCube project(Dimension[] projectOuts){
		//		System.out.println("Projecting " + HelperUtilities.arrayToString(projectOuts) + " out of " + toString());
		//System.out.println("Projecting " + HelperUtilities.arrayToString(projectOuts) + " out of " + HelperUtilities.arrayToString(dimensions));
		Dimension[] newDims = Dimension.setMinusDimensions(this.getDimensions(), projectOuts);
		//System.out.println("New dims: " + HelperUtilities.arrayToString(newDims));
		HyperCube rez = new HyperCube(newDims);
		for (int i = 0; i < rez.getTotalSize(); i++) {
			Dimension.translateLinearToHyper(newDims, i);
			Dimension.setIndexes(newDims, dimensions);
			int max = Integer.MIN_VALUE;
			for (int j = 0; j < Dimension.getTotalSize(projectOuts); j++) {
				Dimension.translateLinearToHyper(projectOuts, j);
				Dimension.setIndexes(projectOuts, dimensions);
				int curval = getValue(dimensions);
				if (curval > max)
					max = curval;
			}
			rez.setValue(i, max);
		}
		return rez;
	}

	//------------------------------------------------------------------//
	/**
	 * convenience method, to eliminate just a single dimension
	 * @param projectOut the dimension to eliminate
	 * @return a new hypercube, with one less dimension
	 */
	public HyperCube project(Dimension projectOut){
		Dimension[] dims = new Dimension[1];
		dims[0] = projectOut;
		return this.project(dims);
	}

	//------------------------------------------------------------------//
	//------------------------------------------------------------------//
	/**
	 * convenience method, to eliminate just a single dimension, based on its variable ID
	 * @param projectOut
	 * @return
	 */
	public HyperCube project(VariableId projectOut){
		Dimension dim = Dimension.findDimension(dimensions, projectOut.getAgentID());
		return this.project(dim);
	}

	//------------------------------------------------------------------//
	//------------------------------------------------------------------//
	/**
	 * @param projectOuts
	 * @return
	 */
	public HyperCube slice(Dimension[] projectOuts){
		//System.out.println("Section along " + HelperUtilities.arrayToString(projectOuts) + " out of " + HelperUtilities.arrayToString(dimensions));
		Dimension[] newDims = Dimension.setMinusDimensions(this.getDimensions(), projectOuts);
		//System.out.println("New dims: " + HelperUtilities.arrayToString(newDims));
		HyperCube rez = new HyperCube(newDims);
		//set the indexes of the fixed variables
		Dimension.setIndexes(projectOuts, dimensions);
		for (int i = 0; i < rez.getTotalSize(); i++) {
			//set the indexes of the remaining variables
			Dimension.translateLinearToHyper(newDims, i);
			Dimension.setIndexes(newDims, dimensions);
			int curval = getValue(dimensions);
			rez.setValue(i, curval);
		}
		return rez;
	}

	//------------------------------------------------------------------//
	/**
	 * convenience method, to slice just through a single dimension
	 * @param sliceout
	 * @return
	 */
	public HyperCube slice(Dimension sliceout){
		Dimension[] dims = new Dimension[1];
		dims[0] = sliceout;
		return this.slice(dims);
	}

	//------------------------------------------------------------------//
	//------------------------------------------------------------------//
	/**
	 * this retains the minimal values corresponding to each combination of values
	 * of the variables to be projected out
	 * this is useful for computing the lower bounds
	 * @param projectOuts
	 * @return
	 */
	public HyperCube project_min(Dimension[] projectOuts){
		Dimension[] newDims = Dimension.setMinusDimensions(this.getDimensions(), projectOuts);
		HyperCube rez = new HyperCube(newDims);
		for (int i = 0; i < rez.getTotalSize(); i++) {
			Dimension.translateLinearToHyper(newDims, i);
			Dimension.setIndexes(newDims, dimensions);
			int min = Integer.MAX_VALUE;
			for (int j = 0; j < Dimension.getTotalSize(projectOuts); j++) {
				Dimension.translateLinearToHyper(projectOuts, j);
				Dimension.setIndexes(projectOuts, dimensions);
				int curval = getValue(dimensions);
				if (curval < min)
					min = curval;
			}
			rez.setValue(i, min);
		}
		return rez;
	}

	//	------------------------------------------------------------------//
	public HyperCube project_bounds(Dimension[] projectOuts){
		Dimension[] newDims = null;
		HyperCube rez = null;
		if (!this.hasDimension(boundDimension)) {
			Dimension boundDimension = new Dimension(-1, 2);
			//adding a special "dimension" with just two values, corresponding to lower/upper bounds
			//all the projectOuts dimensions are replaced with a single dimension
			Dimension[] tmpdims = Dimension.setMinusDimensions(this.getDimensions(), projectOuts);
			newDims = new Dimension[tmpdims.length + 1];
			//first dimension is the bounds dimension
			newDims[0] = boundDimension;
			HelperUtilities.copyArrayOffset(tmpdims, newDims, 1);
			HyperCube hcub = project(projectOuts);
			HyperCube hclb = project_min(projectOuts);
			rez = new HyperCube(newDims);
			//copy at the beginning of the array the values corresponding to dummy_dim=0 (upper bound)
			HelperUtilities.copyArray(hcub.values, rez.values);
			//copy at the end of the array the values corresponding to dummy_dim=1 (lower bound)
			HelperUtilities.copyArrayOffset(hclb.values, rez.values, hcub.values.length);
		} else {
			//STRANGE.. to investigate
			rez = project(projectOuts);
		}
		return rez;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	public int getTotalSize(){
		return Dimension.getTotalSize(dimensions);
	}

	//	------------------------------------------------------------------//
	public int getNumberOfDimensions(){
		return dimensions.length;
	}

	//	------------------------------------------------------------------//
	public int getNumberOfRealDimensions(){
		if (!hasDimension(boundDimension))
			return dimensions.length;
		else
			return dimensions.length - 1;
	}

	//	------------------------------------------------------------------//
	public String toString(){
		if (dimensions.length <= 0)
			return null;
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < dimensions.length; i++) {
			sb.append(dimensions[i].toString() + "\t");
		}
		sb.setCharAt(sb.length() - 1, '|');
		for (int i = 0; i < values.length; i++)
			sb.append(values[i] + " ");
		return sb.toString();
	}

	//	------------------------------------------------------------------//
	public static HyperCube fromString(String src){
		if (src == null)
			return null;
		src.trim();
		if (src.length() == 0)
			return null;
		String[] parts = src.split("\\|");
		if (parts.length == 2) {
			//build the dimension set
			String[] dims = parts[0].split("\\s");
			Dimension[] dimens = new Dimension[dims.length];
			//            System.out.println(dimens.length + " dimensions found");
			for (int i = 0; i < dims.length; i++) {
				dimens[i] = Dimension.fromString(dims[i]);
				//                System.out.println(dimens[i].toString());
			}
			//build the value matrix
			String[] valsS = parts[1].split("\\s");
			int[] vals = new int[valsS.length];
			for (int i = 0; i < vals.length; i++) {
				vals[i] = Integer.parseInt(valsS[i]);
			}
			return new HyperCube(dimens, vals);
		} else {
			System.out.println(parts.length + " parts!!!");
			return null;
		}
	}

	//	------------------------------------------------------------------//
	public boolean hasDimension(Dimension dim){
		return HelperUtilities.arrayContains(dimensions, dim);
	}

	//	------------------------------------------------------------------//
	public String toStringVectors(){
		if (dimensions.length == 0)//that is, this is just a single value 
			return String.valueOf(values[0]);
		int[] indexes = new int[dimensions.length];
		for (int i = 0; i < dimensions.length; i++) {
			indexes[i] = dimensions[i].getCurrentIndex();
		}
		StringBuffer rez = new StringBuffer();
		Dimension smallDim = dimensions[dimensions.length - 1];
		int vectSize = smallDim.getSize();
		int howmvectors = getTotalSize() / vectSize;
		Dimension[] tmpdims = new Dimension[dimensions.length - 1];
		for (int i = 0; i < tmpdims.length; i++) {
			tmpdims[i] = new Dimension(dimensions[i].getId(), dimensions[i].getSize());
		}
		rez.append(HelperUtilities.arrayToString(tmpdims) + "\t");
		for (int i = 0; i < vectSize; i++) {
			smallDim.setCurrentIndex(i);
			rez.append(smallDim.toString() + "\t");
		}
		rez.append("\n-----------------------------------------------------------------\n");
		for (int i = 0; i < howmvectors; i++) {
			Dimension.translateLinearToHyper(tmpdims, i);
			rez.append(HelperUtilities.arrayToString(tmpdims));
			rez.append("\t" + HelperUtilities.arrayToString(values, i * vectSize, (i + 1) * vectSize - 1) + "\n");
			/*            for (int j = 0; j < vectSize; j++)
			 rez.append("\t" + values[i * vectSize + j]);
			 rez.append("\n");
			 //		    rez.append(this.section(tmpdims).toString());

			 */
		}
		for (int i = 0; i < dimensions.length; i++) {
			dimensions[i].setCurrentIndex(indexes[i]);
		}
		return rez.toString();
	}

	//	------------------------------------------------------------------//

	/**
	 * @param dimension
	 * @return
	 */
	public int getDominantValue(Dimension dimension){
		int domNdx = -1;
		Dimension[] tmpDims = { dimension };
		Dimension[] otherDims = Dimension.setMinusDimensions(this.getDimensions(), tmpDims);
		for (int i = 0; i < Dimension.getTotalSize(otherDims); i++) {
			//initialize the other dimensions to the current value
			Dimension.translateLinearToHyper(otherDims, i);
			//section the HC along the current value of the other dimensions
			HyperCube tmpOptimum = this.slice(otherDims);
			//I am finding the optimal assignment for this dimension, given the current section
			//given that otherDims=alldims - dimension, this should have as a dimension only "dimension"
			Dimension[] optAssignment = tmpOptimum.optimalAssignment();
			if (domNdx == -1)
				domNdx = optAssignment[0].getCurrentIndex();
			else if (domNdx != optAssignment[0].getCurrentIndex())
				return -1;
		}
		return domNdx;
	}

	//	------------------------------------------------------------------//
	public double getMaximalDelta(){
		//this returns the maximal percentage off that can be obtained for the (UB-LB)/UB, for each tuple in the hypercube
		System.out.println("Max DELTA computation in " + HelperUtilities.arrayToString(dimensions));
		if (!hasDimension(boundDimension)) {//this means that this is obtained through exact computation, thus no error
			return 0;
		}
		double maxdelta = 0;
		Dimension[] realDims = Dimension.setMinusDimensions(dimensions, boundDimension);
		Dimension boundDimens = Dimension.findDimension(dimensions, boundDimension.getId());
		int howmanytuples = Dimension.getTotalSize(realDims);
		for (int i = 0; i < howmanytuples; i++) {
			Dimension.translateLinearToHyper(realDims, i);//setup the real dimensions
			boundDimens.setCurrentIndex(0);//this is for the max
			double maxval = getValue(dimensions);
			boundDimens.setCurrentIndex(1);//this is for the min
			double minval = getValue(dimensions);
			double perc = 0.1;
			perc = (1 - minval / maxval) * 100;
			if (perc > maxdelta)
				maxdelta = perc;
		}
		System.out.println("Max DELTA computation in " + HelperUtilities.arrayToString(dimensions) + " -> " + maxdelta);
		return maxdelta;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	public Dimension[] optimalAssignment(){
		Dimension[] result = new Dimension[dimensions.length];
		for (int i = 0; i < dimensions.length; i++) {
			result[i] = new Dimension(dimensions[i].getId(), dimensions[i].getSize());
		}
		int maxVal = Integer.MIN_VALUE;
		int maxNdx = -1;
		for (int i = 0; i < values.length; i++) {
			if (values[i] > maxVal) {
				maxVal = values[i];
				maxNdx = i;
			}
		}
		Dimension.translateLinearToHyper(result, maxNdx);
		return result;
	}

	//	------------------------------------------------------------------//
	public int optimalValue(){
		if (dimensions.length == 0)
			return values[0];
		int rez = getValue(optimalAssignment());
		return rez;
	}

	//	------------------------------------------------------------------//

	/**
	 * @param dimension
	 * @return
	 */
	public Vector getDominatedValues(Dimension dimension){
		Vector result = new Vector();
		int counters[] = new int[dimension.getSize()];
		Dimension[] tmpDims = { dimension };
		Dimension[] otherDims = Dimension.setMinusDimensions(this.getDimensions(), tmpDims);
		for (int i = 0; i < Dimension.getTotalSize(otherDims); i++) {
			//initialize the other dimensions to the current value
			Dimension.translateLinearToHyper(otherDims, i);
			//section the HC along the current value of the other dimensions
			HyperCube tmpOptimum = this.slice(otherDims);
			//I am finding the optimal assignment for this dimension, given the current section
			//given that otherDims=alldims - dimension, this should have as a dimension only "dimension"
			Dimension[] optAssignment = tmpOptimum.optimalAssignment();
			counters[optAssignment[0].getCurrentIndex()]++;
		}
		for (int i = 0; i < dimension.getSize(); i++) {
			if (counters[i] == 0)
				result.add(new Integer(i));
		}
		return result;
	}

	//	------------------------------------------------------------------//
	//	------------------------------------------------------------------//
	public Dimension[] selectDimensions(int maxCount, int idparent){
		//this returns the maximal set of dimensions I can keep out of this message
		Dimension[] rez = null;
		if (maxCount >= getNumberOfRealDimensions())//If I can keep all dims
			//I simply return all my dims 
			return dimensions;
		else {
			//          that is, if I have to cut some of my dimensions, maxCount is how many I can keep
			int cntRez = 0;
			Dimension parentDim = Dimension.findDimension(dimensions, idparent);
			if (parentDim == null) {
				System.out.println("Could not find the parent dimension: " + parentDim + " in " + HelperUtilities.arrayToString(dimensions));
				System.exit(1);
			}
			if (this.hasDimension(boundDimension)) {
				rez = new Dimension[maxCount + 1];
				rez[0] = boundDimension;
				rez[1] = parentDim;
				cntRez = 2;
			} else {
				rez = new Dimension[maxCount];
				rez[0] = parentDim;
				cntRez = 1;
			}

			int toAdd = maxCount - 1;
			int cntDims = 0;
			//		for (int i = 0; i < maxCount; i++)
			while (toAdd > 0) {
				if (!dimensions[cntDims].equals(boundDimension) && !dimensions[cntDims].equals(parentDim)) {
					rez[cntRez] = dimensions[cntDims];
					cntRez++;
					toAdd--;
				}
				cntDims++;
			}
			return rez;
		}
	}

	//	------------------------------------------------------------------//
	public boolean isSatisfiable(){
		for (int i = 0; i < values.length; i++)
			if (values[i] >= 0)
				return true;
		return false;
	}

	//	------------------------------------------------------------------//
	public HyperCube getUpperBound(){
		Dimension[] tmpDims = { boundDimension };
		//get the upper bounds
		tmpDims[0].setCurrentIndex(0);
		HyperCube upperBounds = slice(tmpDims);
		return upperBounds;
	}

	//	------------------------------------------------------------------//
	public HyperCube getLowerBound(){
		Dimension[] tmpDims = { boundDimension };
		//get the upper bounds
		tmpDims[0].setCurrentIndex(1);
		HyperCube lowerBounds = slice(tmpDims);
		return lowerBounds;
	}

	//	------------------------------------------------------------------//
	/**
	 * @return Returns the context.
	 */
	public MessageContext getContext(){
		return context;
	}

	/**
	 * @param context The context to set.
	 */
	public void setContext(MessageContext context){
		this.context = context;
	}

	public boolean hasContext(){
		return context != null;
	}

	//	------------------------------------------------------------------//

	public Dimension getBoundDimension(){
		return boundDimension;
	}
}
