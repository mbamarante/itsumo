// OpItem
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
 * A pair of a genetic operator and its weight (used to bias selection
 * between competing operators).
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class OpItem implements Comparable, Serializable
{
    protected double weight;
    protected GenOp oper;

    public OpItem()
    {
    }

    public OpItem( double weighting, GenOp op )
    {
        weight = weighting;
        oper = op;
    }
    
    public int compareTo( Object other )
    {
        OpItem otherO = (OpItem) other;

        if (weight > otherO.weight)
            return 1;
        else if (weight < otherO.weight)
            return -1;
        else
            return 0;
    }

    public void setOp( GenOp newOp )
    {
        oper = newOp;
    }
    
    public GenOp getOp()
    {
        return oper;
    }
    
    public void setWeight( double weighting )
    {
        weight = weighting;
    }
    
    public double getWeight()
    {
        return weight;
    }
}
