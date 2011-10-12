// Population
// Copyright (c) 1998-2003 Matthew Faupel
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library Public License for more details.
//
// You should have received a copy of the GNU Library Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// This Java class is inspired in part by the GAGS C++ library which I
// presume to be copyright J. J. Merelo <jmerelo@kal-el.ugr.es>.

package com.micropraxis.gajit;

import java.io.Serializable;
import com.micropraxis.util.ListIterator;
import com.micropraxis.util.RandomInt;
import com.micropraxis.util.SimpleList;
import com.micropraxis.util.SortedList;

/**
 * A basic class for representing a population of chromosomes.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class Population extends SimpleList implements Serializable
{
    /**
     * Percentage of population carried forward unchanged from each
     * generation.
     */
    protected double elitePercent;

    /**
     * Percentage deemed unfit for reproduction
     */
    protected double cullPercent;

    /**
     * Operators used to modify the current generation when breeding the
     * next.
     */
    protected SortedList opList;

    /**
     * Index in our list of the first element that has been generated from
     * new for this generation rather than copied from the previous.
     */
    protected int newIndex;

    /**
     * Cache of the operators in the above list for efficiency, along with
     * details of their weightings to allow one to be selected.
     */
    protected OpItem [] operators;
    protected double [] cumulativeWeight;
    protected double totalWeight;
    
    public Population()
    {
    }

    public Population( int popSize, int geneSize, int minGenes, 
        int maxGenes )
    {
        this( popSize, geneSize, minGenes, maxGenes, 0 );
    }
    
    /**
     * Constructor.  If a non-zero mutation rate is given, the default
     * operators of mutation (at the given rate) and two point crossover are
     * associated with the population automatically.
     */
    public Population( int popSize, int geneSize, int minGenes, 
        int maxGenes, double mutRate )
    {
        createPop( popSize, geneSize, minGenes, maxGenes ); 

        newIndex = 0;
        operators = null;
        cumulativeWeight = null;
        totalWeight = 0.0;
        opList = new SortedList();

        // Create default genetic operators, mutation and 2-pt xOver

        if (mutRate != 0.0)
        {	
            GenOp mutator = new MutOp( mutRate );
            GenOp xOver = new XOverPtOp( 2 ); // 2-pt xOver

            // 1 is the default "priority"

            opList.add( new OpItem( 1, mutator ) );
            opList.add( new OpItem( 1, xOver ) );
        }
    }

    /**
     * Empty the population.
     */
    public void clear()
    {
        // Overridden to reset the new items pointer.

        super.clear();
        newIndex = 0;
    }

    /**
     * Return an iterator over the whole population.
     */
    public ListIterator iteratorAll()
    {
        return listIterator();
    }

    /**
     * Return an iterator over the portion of the population that is new for
     * this generation.  That is, the portion that has not yet been evaluated
     * for fitness.
     */
    public ListIterator iteratorNewOnly()
    {
        return listIterator( newIndex );
    }

    /**
     * Provide a genetic modifier operator for use on this population, and
     * the weighting it has for the chances of it being selected.
     */
    public void addOp( GenOp genOp, double weight )
    {
        opList.add( new OpItem( weight, genOp ) );
        operators = null;
    }

    /**
     * Define the proportion of the population carried forward unchanged
     * between generations.
     */
    public void setEliteRate( double perc )
    {
        if (perc < 0.0)
            elitePercent = 0.0;
        else if (perc > 1.0)
            elitePercent = 1.0;
        else
            elitePercent = perc;
    }

    /**
     * Define the proportion of the population carried forward unchanged
     * between generations.
     */
    public void setCullRate( double perc )
    {
        if (perc < 0.0)
            cullPercent = 0.0;
        else if (perc > 1.0)
            cullPercent = 1.0;
        else
            cullPercent = perc;
    }

    /**
     * A new generation is created by carrying over an elite part unchanged
     * and then generating the remainder from breeding, excluding the worst
     * of the current generation from breeding at a rate defined by
     * setCullRate.
     */
    public void newGeneration()
    {
        // Create new population by copying the elite and then generating
        // the required remainder.

        SortedList orderedPop = new SortedList( this );
        ChromItem [] oldPop = new ChromItem[orderedPop.size()];
        int totPop = oldPop.length;
        int elite = (int) (totPop * elitePercent);
        int culled = (int) (totPop * cullPercent);

        // System.out.println( "Total population: " + totPop );

        oldPop = (ChromItem []) orderedPop.toArray( oldPop );
        clear();

        // Chromosomes for the new generation are created by duplication
        // rather than just being transferred to ensure that they remain
        // unchanged by the breeding program carried out later

        for (int i = totPop - elite; i < totPop; ++i)
            add( new ChromItem( oldPop[i] ) );
        newIndex = elite;

        for (int i = elite; i < totPop; ++i)
        {
            Chrom newChrom = new Chrom( selectChromosome( oldPop, culled, 
                totPop - culled ) );
            GenOp operator = selectOperator();

            // Apply genetic operator to chosen chromosome

            // System.out.println( "Modifying " + newChrom + " with " +
            //                      operator );

            if (!operator.isBinaryOp())
                operator.apply( newChrom );
            else
            {
                // choose another random gene

                Chrom mate = new Chrom( selectChromosome( oldPop, culled, 
                    totPop - culled ) );

                // System.out.println( "...and " + mate );

                operator.apply( newChrom, mate );
            }

            // System.out.println( "Result: " + newChrom );

            add( new ChromItem( 0, newChrom ) );
        }
    }

    /**
     * Eliminate the current population and create a new one		
     */
    public void reset( int geneSize, int minGenes, int maxGenes )
    {
        int totPop = size();

        clear();
        createPop( totPop, geneSize, minGenes, maxGenes );	
    }

    /**
     * Create new population
     */
    protected void createPop( int popSize, int geneSize, int minGenes,
        int maxGenes )
    {
        for (int i = 0; i < popSize; ++i)
        {
            int len = minGenes + RandomInt.next( maxGenes - minGenes + 1 );
            Chrom newChrom = new Chrom( len * geneSize );

            add( new ChromItem( 0, newChrom ) );
        }
    }

    /**
     * Randomly select a chromosome to breed with.  The routine selects a
     * chromosome from <code>chroms</code> from within the range defined by
     * <code>first</code> and <code>length</code>.  This implementation
     * simply selects a valid chromosome at random, however the routine
     * could be overridden by subclasses, e.g. to bias the selection
     * according to the weighting of the chromosomes, vary the algorithm
     * according to the maturity of the population and so on.
     *
     * @param chroms array of ChromItems to select a chromosome from.  The
     *  array is guaranteed to be ordered from worst to best fitness.
     * @param first the first valid element in the array
     * @param length the number of valid elements in the array
     * @return a chromosome chosen from the valid elements
     */
    protected Chrom selectChromosome( ChromItem [] chroms, int first,
        int length )
    {
        return chroms[RandomInt.next( length ) + first].getChrom();
    }

    /**
     * Randomly select a genetic operator from those associated with this
     * population.
     */
    protected GenOp selectOperator()
    {
        if (operators == null || operators.length != opList.size())
        {
            // Create the cache if it's currently invalid

            if (operators == null)
                operators = new OpItem[opList.size()];
            operators = (OpItem []) opList.toArray( operators );
            cumulativeWeight = new double[operators.length];
            totalWeight = 0;

            for (int i = 0; i < operators.length; ++i)
            {
                double opWeight = operators[i].getWeight();

                cumulativeWeight[i] = opWeight + totalWeight;
                totalWeight += opWeight;
            }
        }

        // generate random operator choice based on the weightings of each
        // operator 

        double chosenWeight = Math.random() * totalWeight;
        int min = 0;
        int max = operators.length - 1;
        int chosen = (int) ((chosenWeight / totalWeight) * max);

        do
        {
            if (chosenWeight > cumulativeWeight[chosen])
            {
                min = chosen + 1;
                chosen += (max - chosen - 1)/2 + 1;
            }
            else if (chosen != 0 &&
                chosenWeight <= cumulativeWeight[chosen-1])
            {
                max = chosen - 1;
                chosen -= (chosen - min - 1)/2 + 1;
            }
            else break;
        }
        while (true);

        return operators[chosen].getOp();
    }
}
