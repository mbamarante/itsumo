// ListIterator
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

package com.micropraxis.util;

/**
 * This interface is implemented by iterators over lists.  It is identical
 * to the JDK 1.2 ListIterator interface (except for the exceptions :-) to
 * aid porting at a later date.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public interface ListIterator extends Iterator
{
    /**
     * Inserts the specified element into the List.
     */
    public void add( Object o );
       
    /**
     * Returns true if this ListIterator has more elements when traversing
     * the List in the forward direction. 
     */
    public boolean hasNext();

    /**
     * Returns true if this ListIterator has more elements when traversing
     * the list in the reverse direction. 
     */
    public boolean hasPrevious();
    
    /**
     * Returns the next element in the List. 
     */
    public Object next();
    
    /**
     * Returns the index of the element that would be returned by a
     * subsequent call to next.  
     */
    public int nextIndex();
    
    /**
     * Returns the previous element in the List. 
     */
    public Object previous();
    
    /**
     * Returns the index of the element that would be returned by a
     * subsequent call to previous. 
     */
    public int previousIndex();
    
    /** 
     * Removes from the List the last element that was returned by next or
     * previous.  
     */
    public void remove();
    
    /**
     * Replaces the last element returned by next or previous with the
     * specified element. 
     */
    public void set( Object o );
}
