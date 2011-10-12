package optapo;

import java.util.Vector;

import com.micropraxis.gajit.Chrom;
import com.micropraxis.gajit.ChromItem;
import com.micropraxis.gajit.DupOp;
import com.micropraxis.gajit.FixView;
import com.micropraxis.gajit.Population;
import com.micropraxis.gajit.View;
import com.micropraxis.util.ExtendedBitSet;
import com.micropraxis.util.Iterator;
import com.micropraxis.util.ListIterator;
import com.micropraxis.util.SortedList;

public class geneticalgorithm_old {

	public Vector optimal;
	public Vector constraints;
	int NUMGENES = 0;
	int excludedNUMGENES = 0;
    int SIZEGENES = 3;
    static final int GENERATIONS = 100;
    static final int NUMCHROMS = 200;

	
	public geneticalgorithm_old(Vector vector, Vector constraints, int optimalCost) {
		this.constraints = constraints;
		System.out.println(" dentro do GA "); 
		NUMGENES = vector.size();
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>> "+optimalCost+" === "+this.cost(vector));
		
		for (int i = 0; i < vector.size(); i++) {

			if (((agent)vector.elementAt(i)).name.equals("ND9") || ((agent)vector.elementAt(i)).name.equals("ND16")) {

				NUMGENES--;
				excludedNUMGENES++;

			}
		}
		System.out.println("ex genes: "+excludedNUMGENES);
		System.out.println("to genes: "+NUMGENES);
		
		if (optimalCost != this.cost(vector)) {			
			ga(vector);
		}
		else this.optimal = vector;
	}
	
	public void ga(Vector tempGoodlist){
		
		
	    if (!tempGoodlist.isEmpty()) {
	    		Population people = new Population( NUMCHROMS, SIZEGENES, NUMGENES,
	                NUMGENES, 1.0/(SIZEGENES*NUMGENES) );
	            View vista = new FixView( SIZEGENES, 0.0, 7.0 );
	            DupOp adder = new DupOp( SIZEGENES );

	            people.setEliteRate( 0.05 );
	            people.setCullRate( 0.4 );
	            people.addOp( adder, 0.1 );

	            for (int i = 0; i < GENERATIONS; ++i)
	            {
	                Iterator censo;

	                censo = people.iteratorNewOnly();
	    	
	                while (censo.hasNext())
	                {
	                    double val = 0.0;
	                    ChromItem tmp = (ChromItem) censo.next();
	                    Chrom chrom = tmp.getChrom();
	                    int geneCount = vista.size( chrom );                
	                   
	                    
	                    Vector temp = (Vector)this.vectorOfAgentsClone(tempGoodlist);

	                    for (int j = 0; j < NUMGENES + excludedNUMGENES; ++j) {

	                    	if (((agent)temp.elementAt(j)).name.equals("ND9") || ((agent)temp.elementAt(j)).name.equals("ND16")) {
	                    		((agent)temp.elementAt(j)).value = "0";
	                    	}
	                    	else ((agent)temp.elementAt(j)).value = Integer.toString(((Double) vista.getGene(chrom, j)).intValue()+1);
	
	                    }

	                }
	                if (i < GENERATIONS - 1)
	                    people.newGeneration();
	            }

	            SortedList results = new SortedList( people );
	            ListIterator iter = results.listIterator( results.size() );

	                ChromItem item = (ChromItem) iter.previous();
	                Chrom chrom = item.getChrom();

	                
	                
	                Vector temp = (Vector)this.vectorOfAgentsClone(tempGoodlist);
	                
	                for (int j = 0; j < NUMGENES + excludedNUMGENES; ++j) {
	                	
	                	if (((agent)temp.elementAt(j)).name.equals("ND9") || ((agent)temp.elementAt(j)).name.equals("ND16")) {
                    		((agent)temp.elementAt(j)).value = "0";
                    	}

                    	else 
                    		((agent)temp.elementAt(j)).value = Integer.toString(((Double) vista.getGene(chrom, j)).intValue()+1);

                    }
                    this.optimal = temp;
	           // }
	        }
	}
	
	public Vector changeAgentDomain(Vector list, String domain, String agentName){
		for (int i = 0; i < list.size(); i++) {
			if (((agent)list.elementAt(i)).name.equals(agentName)) {
				((agent)list.elementAt(i)).value = domain;
			}	
		}
		return list;
	}
	
	public int cost(Vector vector){
		Vector tempVector = (Vector)vector.clone();
		int tempCost = 0;
		for (int i = 0; i < vector.size(); i++) {
				
			for (int j = 0;	j < tempVector.size(); j++) {
				if (!((agent)vector.elementAt(i)).equals((agent)tempVector.elementAt(j))){

					constraint tempConstraint = this.getConstraint(((agent)vector.elementAt(i)),((agent)tempVector.elementAt(j)));
					if (tempConstraint != null) {
						tempCost += tempConstraint.getCost(((agent)vector.elementAt(i)).value, ((agent)tempVector.elementAt(j)).value);

					}

				}
			}
		}
		return tempCost;
	}
	
	public constraint getConstraint(agent a, agent b){
		for (int i = 0; i < constraints.size(); i++) {
			if ((((constraint)constraints.elementAt(i)).variable.name.equals(a.name) &&
				((constraint)constraints.elementAt(i)).otherVariable.name.equals(b.name))) {
					return (constraint)constraints.elementAt(i);
			}
		}
		return null;
	}
	public Vector vectorOfAgentsClone(Vector temp) {
		Vector t = new Vector();
		for (int i = 0; i < temp.size(); i++) {
			t.addElement(((agent)temp.elementAt(i)).clone());		
		}
		return (Vector)t.clone();
	}
	
}
