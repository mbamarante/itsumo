// SortedList
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

import java.io.Serializable;

/**
 * Extends SimpleList to hold elements in ascending (partial) order based on
 * the result of calling compareTo on the object or using a Comparator
 * provided on construction.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class SortedList extends SimpleList implements Serializable
{
    Comparator comp;

    public SortedList()
    {
        comp = null;
    }

    public SortedList( Comparator comparator )
    {
        comp = comparator;
    }

    public SortedList( Collection c )
    {
        super( c );
    }

    public SortedList( Comparator comparator, Collection c )
    {
        comp = comparator;
        addAll( c );
    }

    /**
     * Returns a ListIterator of the elements in this list, starting at
     * the specified position in the list.
     */
    public ListIterator listIterator( int index )
    {
        return new SortedListIterator( index );
    }

    /**
     * Adds the specified element to the list.  It will be inserted after
     * the last existing element that has a lower or equal value.
     *
     * @param item element to add.
     * @return true (as per the general contract of Collection.add)
     */
    public boolean add( Object item )
    {
        add( numElems, item );
        return true;
    }

    /**
     * Adds the specified element at the given index.  If adding the item at
     * the index given would result in the partial ordering of the list
     * being destroyed, the element migrates up or down the list as
     * appropriate until the ordering is rest
     *
     * @param index zero-based index of element to insert before.
     * @param item element to add.
     */
    public void add( int index, Object item )
    {
        dirtyIterators();

        // JDK list throws an exception; we'll just normalise for now

        if (index < 0)
            index = 0;
        else if (index > numElems)
            index = numElems;

        Node insertBefore;
        Node temp = new Node();
        
        temp.item = item;

        // Find roughly where we want to insert the new item

        if (index * 2 > numElems)
        {
            insertBefore = tail;
            for (int i = numElems; i > index; --i)
                insertBefore = insertBefore.prev;
        }
        else
        {
            insertBefore = head.next;
            for (int i = 0; i < index; ++i)
                insertBefore = insertBefore.next;
        }

        // Then ensure that we retain the partial order

        while (compare( item, insertBefore.item ) > 0)
            insertBefore = insertBefore.next;
        while (compare( item, insertBefore.prev.item ) < 0)
            insertBefore = insertBefore.prev;
        
        temp.next = insertBefore;
        temp.prev = insertBefore.prev;
        temp.prev.next = temp;
        insertBefore.prev = temp;

        ++numElems;
    }

    /**
     * Internal routine for comparing list items.  Returns -1, 0 or 1 to
     * indicate that the first object is less, equal to or greater than the
     * second.  It returns 0 if the second object is null to ensure the
     * correct behaviour when inserting at the beginning or end of the list.
     */
    protected int compare( Object o1, Object o2 )
    {
        if (o2 == null)
            return 0;
        else if (comp != null)
            return comp.compare( o1, o2 );
        else
            return ((Comparable) o1).compareTo( o2 );
    }

    /**
     * A fail fast iterator for SortedList.  If any operation is performed
     * on the associated list that modifies the list other than through this
     * iterator, the iterator will throw an exception on next use.
     */
    protected class SortedListIterator extends SimpleListIterator
    {
        SortedListIterator( int idx )
        {
            super( idx );
        }

        /**
         * Inserts the specified element into the list, ensuring that we
         * retain partial order.  This means that if it is inserted into the
         * wrong place, it won't be the next item to be read by the
         * iterator. 
         */
        public void add( Object item )
        {
            check();
            firstIter.dirty( this );
            
            Node newNode = new Node();
            Node insertBefore = currentPos;
            boolean lower = false;

            while (compare( item, insertBefore.item ) > 0)
                insertBefore = insertBefore.next;
            while (compare( item, insertBefore.prev.item ) < 0)
            {
                lower = true;
                insertBefore = insertBefore.prev;
            }

            newNode.item = item;
            newNode.next = insertBefore;
            newNode.prev = insertBefore.prev;
            insertBefore.prev = newNode;
            newNode.prev.next = newNode;
            ++numElems;
            if (currentPos == insertBefore)
                currentPos = newNode;
            if (lower)
                ++index;
            forward = false;
        }

        /**
         * Replaces the last element returned by next or previous with the
         * specified element. If the replacement doesn't have the same value
         * as the replaced item, it will be reshuffled in the list.
         */
        public void set( Object item )
        {
            remove();
            add( item );
        }
    }
}
