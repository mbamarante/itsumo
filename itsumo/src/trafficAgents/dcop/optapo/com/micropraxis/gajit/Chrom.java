// Chrom
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
import java.util.Random;
import com.micropraxis.util.ExtendedBitSet;

/**
 * A class that represents a chromosome and provides various appropriate
 * ways of constructing and manipulating the chromosome.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class Chrom extends ExtendedBitSet
    implements Serializable, Cloneable
{
    /**
     * Unique id for the chromosome
     */
    protected long thisId;

    /**
     * Unique id generator source
     */
    protected static long lastId = 0;

    /**
     * Default constructor
     */
    public Chrom() 
    {
    }
  
    /**
     * Creates a random chromosome with a given length.
     *
     * @param length length of chromosome
     */
    public Chrom( int length )
    {
        super( length, 0 );
        thisId = ++lastId;
    }

    /**
     * Creates a chromosome from an existing set of bits
     *
     * @param bitSet set of bits to use
     */
    public Chrom( ExtendedBitSet bitSet )
    {
        super( bitSet );
        thisId = ++lastId;
    }

    /**
     * Creates a chromosome by copying another.
     *
     * @param parent chromosome to copy
     */
    public Chrom( Chrom parent )
    {
        this( (ExtendedBitSet) parent );
    }

    /**
     * Creates a chromosome from two parent chromosome, randomly selecting
     * bits from each.
     *
     * @param parent1 mom
     * @param parent2 pop
     */
    public Chrom( Chrom parent1, Chrom parent2 )
    {
        this( parent1, parent2, 0.5 );
    }

    /**
     * Creates a chromosome from two parent chromosomes, selecting bits
     * from each according to the given crossover rate.  A value of 0.0
     * means all bits come from parent1, 1.0 means that all bits come
     * from parent2.  The child is always of equal length to parent1.
     *
     * @param parent1 mom
     * @param parent2 pop
     * @param xOverRate value between 0.0 and 1.0 dictating crossover rate
     */
    public Chrom( Chrom parent1, Chrom parent2, double xOverRate ) 
    {
        this( parent1 );

        if (xOverRate > 0)
            crossover( parent2, xOverRate );
    }

    /**
     * Creates a chromosome by mutating another.
     *
     * @param parent chromosome to copy
     * @param mutationRate rate of mutation. 0.0 = no mutation, 1.0 is
     *  equivalent to inverting the entire chromosome.
     */
    public Chrom( Chrom parent, double mutationRate )
    {
        this( parent );

        if (mutationRate > 0.0)
            mutate( mutationRate );
    }

    /**
     * Copy the contents of another chromosome
     *
     * @param other the chromosome to copy
     * @return this
     */
    public Chrom copy( Chrom other ) 
    {
        super.copy( (ExtendedBitSet) other );

        return this;
    }
  
    /**
     * Create and return a copy of this chromosome.
     *
     * @return a copy of this chromosome
     */
    public Object clone()
    {
        return new Chrom( this );
    }

    /**
     * Mutate the chromosome with given probability of each bit changing.
     *
     * @param mutationRate bit change probability from 0.0 to 1.0.
     * @return this
     */
    Chrom mutate( double mutationRate )
    {
        for (int i = 0; i < lenBits; ++i)
        {
            if (mutationRate >= 1.0 || Math.random() < mutationRate)
                invert( i );
        }

        return this;
    }

    /**
     * Uniform crossover. Performs a uniform crossover of this chromosome
     * with another with given probability of selecting each bit from the
     * other chromosome.
     *
     * @param other other chromosome
     * @param xOverRate crossover rate (0.0 <= rate <= 1.0)
     * @return this
     */
    Chrom crossover( Chrom other, double xOverRate ) 
    {
        // Use the shorter of the two

        int limit = (lenBits < other.lenBits) ? lenBits : other.lenBits;

        for (int i = 0; i < limit; ++i)
        {
            if (xOverRate >= 1.0 || Math.random() < xOverRate)
                setBooleanAt( i, other.get( i ) );
        }

        return this;
    } 
  
    /**
     * Swap two chunks of bits within the chromosome.  If the chunks
     * overlap the contained values will be extracted correctly, but then
     * chunk1 will be written back to chunk2's position after chunk2 is
     * written to chunk1's position.
     *
     * @param chunk1 Offset of the first chunk
     * @param chunk2 Offset of the second chunk
     * @param length Length of the chunks
     * @return this
     */
    Chrom transposeChunk( int chunk1, int chunk2, int length )
    {
        ExtendedBitSet bs1 = getSubSet( chunk1, length );
        ExtendedBitSet bs2 = getSubSet( chunk2, length );

        setSubSet( chunk1, bs2 );
        setSubSet( chunk2, bs1 );

        return this;
    }

    /**
     * Adds a chunk of bits at the end.  The bits of the new chunk are
     * randomly initialised.
     *
     * @param length length of chunk to add
     * @return this
     */
    Chrom addChunkEnd( int length ) 
    {
        ExtendedBitSet bs = new ExtendedBitSet( length, 0 );
        
        insertSubSet( -1, bs );
        return this;
    } 

    /**
     * Removes a chunk of bits.  <code>length</code> bits at offset
     * <code>offset</code> are removed.
     *
     * @param offset position of chunk
     * @param length length of chunk to remove
     * @return this
     */
    Chrom killChunk( int offset, int length )
    {
        deleteSubSet( offset, length );
        return this;
    }

    /**
     * Duplicate a chunk of bits.  <code>length</code> bits at offset
     * <code>offset</code> are duplicated and the copy is inserted after the
     * original in the chromosome.
     *
     * @param offset position of chunk
     * @param length length of chunk to duplicate
     * @return this
     */
    Chrom dupChunk( int offset, int length )
    {
        ExtendedBitSet bs = getSubSet( offset, length );

        insertSubSet( offset, bs );
        return this;
    }

    public String toString()
    {
        return "[" + thisId + ":" + super.toString() + "]";
    }
}
