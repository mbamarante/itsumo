// ChromItem
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
import com.micropraxis.util.Comparable;

/**
 * A pair of a chromosome and its fitness rating.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class ChromItem implements Comparable, Serializable
{
    // We currently need both fitness and doubleFitness because the
    // JDK1.1 number classes (Integer etc.) don't implement Comparable.
    // This can be done away with when JDK1.2 becomes widespread.

    protected Comparable fitness;
    protected double doubleFitness;
    protected Chrom chrom;

    public ChromItem()
    {
    }

    public ChromItem( ChromItem other )
    {
        // Deep copy the chromosome so that when a chromosome is inserted
        // into a new generation, its value is unaltered by any breeding it
        // might take part in as part of the old generation (that would
        // affect the correctness of the associated fitness)

        fitness = other.fitness;
        doubleFitness = other.doubleFitness;
        chrom = new Chrom( other.chrom );
    }

    public ChromItem( double fit, Chrom chromosome )
    {
        fitness = null;
        doubleFitness = fit;
        chrom = chromosome;
    }
    
    public ChromItem( Comparable fit, Chrom chromosome )
    {
        fitness = fit;
        doubleFitness = 0.0;
        chrom = chromosome;
    }

    public int compareTo( Object other )
    {
        ChromItem otherC = (ChromItem) other;

        if (fitness == null)
        {
            if (doubleFitness > otherC.doubleFitness)
                return 1;
            else if (doubleFitness < otherC.doubleFitness)
                return -1;
            else
                return 0;
        }
        else
            return fitness.compareTo( ((ChromItem) other).fitness );
    }

    public boolean equals( Object other )
    {
        if (other instanceof ChromItem)
            return (compareTo( (ChromItem) other ) == 0) &&
                chrom.equals( ((ChromItem) other).getChrom() );
        else
            return false;
    }

    public int hashCode()
    {
        if (fitness == null)
            return (int) Double.doubleToLongBits( doubleFitness ) ^
                chrom.hashCode();
        else
            return fitness.hashCode() ^ chrom.hashCode();
    }

    public void setChrom( Chrom newChrom )
    {
        chrom = newChrom;
    }
    
    public Chrom getChrom()
    {
        return chrom;
    }
    
    public void setFitness( double fit )
    {
        fitness = null;
        doubleFitness = fit;
    }
    
    public void setFitness( Comparable fit )
    {
        fitness = fit;
    }
    
    public double getDoubleFitness()
    {
        return doubleFitness;
    }

    public Comparable getFitness()
    {
        return fitness;
    }

    public String toString()
    {
        if (fitness == null)
            return "[" + doubleFitness + "," + chrom.toString() + "]";
        else
            return "[" + fitness.toString() + "," + chrom.toString() + "]";
    }
}
