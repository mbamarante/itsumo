// Collection
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
 * A duplicate of the JDK1.2 Collection interface to aid porting once JDK1.2
 * becomes standard.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public interface Collection
{
    /**
     * Returns the number of elements in this Collection.
     */
    public int size();

    /**
     * Returns true if this Collection contains no elements.
     */
    public boolean isEmpty();

    /**
     * Returns true if this Collection contains the specified element.
     *
     * @param o element whose presence in this Collection is to be tested.
     */
    public boolean contains( Object o );

    /**
     * Returns an Iterator over the elements in this Collection.
     */
    public Iterator iterator();

    /**
     * Returns an array containing all of the elements in this Collection.
     */
    public Object [] toArray();

    /**
     * Returns an array containing all of the elements in this Collection,
     * whose runtime type is that of the specified array.
     *
     * @param a the array into which the elements of the Collection are to
     *          be stored, if it is big enough; otherwise, a new array of the
     *          same runtime type is allocated for this purpose.
     * @return an array containing the elements of the Collection.
     */
    public Object [] toArray( Object [] a );

    /**
     * Ensures that this Collection contains the specified element.
     *
     * @param o element whose presence in this Collection is to be ensured.
     * @return true if the Collection changed as a result of the call.
     */
    public boolean add( Object o );

    /**
     * Removes a single instance of the specified element from this
     * Collection, if it is present.
     *
     * @param o element to be removed from this Collection, if present.
     * @return true if the Collection changed as a result of the call.
     */
    public boolean remove( Object o );

    /**
     * Returns true if this Collection contains all of the elements in the
     * specified Collection.
     */
    public boolean containsAll( Collection c );

    /**
     * Adds all of the elements in the specified Collection to this
     * Collection. 
     *
     * @param c elements to be inserted into this Collection.
     * @return true if this Collection changed as a result of the call.
     */
    public boolean addAll( Collection c );

    /**
     * Removes from this Collection all of its elements that are contained in
     * the specified Collection.
     *
     * @param c elements to be removed from this Collection.
     * @return true if this Collection changed as a result of the call.
     */
    public boolean removeAll( Collection c );

    /**
     * Retains only the elements in this Collection that are contained in the
     * specified Collection.
     *
     * @param c elements to be retained in this Collection.
     * @return true if this Collection changed as a result of the call.
     */
    public boolean retainAll( Collection c );

    /**
     * Removes all of the elements from this Collection.
     */
    public void clear();

    /**
     * Compares the specified Object with this Collection for equality.
     *
     * @param o Object to be compared for equality with this Collection.
     * @return true if the specified Object is equal to this Collection.
     */
    public boolean equals( Object o );

    /**
     * Returns the hash code value for this Collection.
     */
    public int hashCode();
}
