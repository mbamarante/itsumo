// SimpleList
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
import java.lang.reflect.Array;

/**
 * A simple implementation of a list and its iterator class.  It implements
 * a subset of the JDK1.2 List interface in order to simplify porting at a
 * later date.  It has the property of maintaining the order of elements.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class SimpleList implements Collection, Serializable
{
    /**
     * Number of elements
     */
    protected int numElems;

    protected static class Node
    {
        public Object item;
        public Node next;
        public Node prev;
    }

    /**
     * Head and tail of list
     */
    protected Node head;
    protected Node tail;

    /**
     * Head of the list of associated iterators
     */
    protected SimpleListIterator firstIter;

    public SimpleList() 
    {
        head = new Node();
        tail = new Node();
        head.prev = tail.next = null;
        firstIter = null;
        clear();
    }

    public SimpleList( Collection other )
    {
        this();
        addAll( other );
    }

    /**
     * Returns the number of elements in this list.
     */
    public int size() 
    {
        return numElems;
    }
    
    /**
     * Returns true if this list contains no elements.
     */
    public boolean isEmpty()
    {
        return numElems == 0;
    }

    /**
     * Returns true if this list contains the specified element.
     *
     * @param o element whose presence in this list is to be tested.
     */
    public boolean contains( Object toFind )
    {
        boolean isIn = false;

        for (Node elem = head.next; elem != tail && !isIn; elem = elem.next)
        {
            if (toFind.equals( elem.item ))
                isIn = true;
        }

        return isIn;
    }

    /**
     * Returns an Iterator over the elements in this list.
     */
    public Iterator iterator()
    {
        return listIterator();
    }

    /**
     * Returns a ListIterator of the elements in this list.
     */
    public ListIterator listIterator()
    {
        return listIterator( 0 );
    }

    /**
     * Returns a ListIterator of the elements in this list, starting at
     * the specified position in the list.
     */
    public ListIterator listIterator( int index )
    {
        return new SimpleListIterator( index );
    }

    /**
     * Returns an array containing all of the elements in this list.
     */
    public Object [] toArray()
    {
        Object [] retValue = new Object[numElems];
        int index = 0;

        for (Node elem = head.next; elem != tail; elem = elem.next)
            retValue[index++] = elem.item;

        return retValue;
    }

    /**
     * Returns an array containing all of the elements in this list,
     * whose runtime type is that of the specified array.
     *
     * @param a the array into which the elements of the list are to be
     *          stored, if it is big enough; otherwise, a new array of the
     *          same runtime type is allocated for this purpose.
     * @return an array containing the elements of the list.
     */
    public Object [] toArray( Object [] a )
    {
        if (a == null)
            return toArray();
        else
        {
            Object [] retValue = a;
            Class retClass = a.getClass().getComponentType();
            int index = 0;

            for (Node elem = head.next; elem != tail; elem = elem.next)
            {
                if (retClass.isInstance( elem.item ))
                {
                    if (index >= retValue.length)
                    {
                        retValue = (Object []) Array.newInstance(
                            retClass, numElems );
                        System.arraycopy( a, 0, retValue, 0, index );
                    }
                    retValue[index++] = elem.item;
                }
            }

            return retValue;
        }
    }

    /**
     * Adds the specified element.
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
     * Adds the specified element at the given index.
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
        
        temp.next = insertBefore;
        temp.prev = insertBefore.prev;
        temp.prev.next = temp;
        insertBefore.prev = temp;

        ++numElems;
    }

    /**
     * Removes a single instance of the specified element from this
     * Collection, if it is present.
     *
     * @param o element to be removed from this Collection, if present.
     * @return true if the Collection changed as a result of the call.
     */
    public boolean remove( Object o )
    {
        boolean found = false;

        for (Node elem = head.next; elem != tail & !found; elem = elem.next)
        {
            if (o == elem.item)
            {
                dirtyIterators();

                elem.prev.next = elem.next;
                elem.next.prev = elem.prev;
                --numElems;
                found = true;
            }
        }

        return found;
    }

    /**
     * Returns true if this list contains all of the elements in the
     * specified Collection.
     */
    public boolean containsAll( Collection c )
    {
        SimpleList temp = new SimpleList( this );

        temp.removeAll( c );
        
        return numElems - temp.size() == c.size();
    }

    /**
     * Adds all of the elements in the specified Collection to the end of
     * this list.
     *
     * @param other elements to be inserted into this list.
     * @return true if this list changed as a result of the call.
     */
    public boolean addAll( Collection other )
    {
        Iterator iter = other.iterator();

        while (iter.hasNext())
            add( iter.next() );

        return true;
    }

    /**
     * Removes from this list all of its elements that are contained in
     * the specified list.
     *
     * @param c elements to be removed from this list.
     * @return true if this list changed as a result of the call.
     */
    public boolean removeAll( Collection other )
    {
        Iterator iter = other.iterator();
        int oldCount = numElems;

        while (iter.hasNext())
            remove( iter.next() );

        return oldCount != numElems;
    }

    /**
     * Retains only the elements in this Collection that are contained in the
     * specified Collection.
     *
     * @param c elements to be retained in this Collection.
     * @return true if this Collection changed as a result of the call.
     */
    public boolean retainAll( Collection c )
    {
        Iterator iter = c.iterator();
        Object [] elements = toArray();

        while (iter.hasNext())
        {
            Object item = iter.next();

            for (int i = 0; i < elements.length; ++i)
            {
                if (item.equals( elements[i] ))
                {
                    elements[i] = null;
                    break;
                }
            }
        }

        // Delete all unmatched elements

        int i = 0;
        for (Node node = head.next; node != tail; node = node.next)
        {
            if (elements[i++] != null)
            {
                dirtyIterators();
                node.prev.next = node.next;
                node.next.prev = node.prev;
                --numElems;
            }
        }

        return numElems != elements.length;
    }

    /**
     * Removes all of the elements from this Collection.
     */
    public void clear()
    {
        dirtyIterators();

        numElems = 0;
        head.next = tail;
        tail.prev = head;
    }

    public boolean equals( Object other )
    {
        if (other instanceof SimpleList)
        {
            SimpleList list = (SimpleList) other;

            if (numElems != list.numElems)
                return false;
            else if (numElems == 0)
                return true;
            else
                return containsAll( list );
        }
        else
            return false;
    }

    public int hashCode()
    {
        int hash = numElems;

        for (Node elem = head.next; elem != tail; elem = elem.next)
            hash ^= elem.item.hashCode();

        return hash;
    }

    public String toString()
    {
        StringBuffer buf = new StringBuffer();

        buf.append( "(" );
    
        for (Node elem = head.next; elem != tail; elem = elem.next)
        {
            buf.append( elem.item.toString() );
            if (elem.next != tail)
                buf.append( "," );
        }

        buf.append( ")" );

        return buf.toString();
    }

    protected void dirtyIterators()
    {
        if (firstIter != null)
            firstIter.dirty( null );
    }

    /**
     * A fail fast iterator for SimpleList.  If any operation is performed
     * on the associated list that modifies the list other than through this
     * iterator, the iterator will throw an exception on next use.
     */
    protected class SimpleListIterator implements ListIterator
    {
        SimpleListIterator next;
        Node currentPos;
        int index;
        boolean forward;

        SimpleListIterator( int idx )
        {
            forward = false;

            if (idx < 0)
                idx = 0;
            else if (idx > numElems)
                idx = numElems;

            if (idx * 2 > numElems)
            {
                for (currentPos = tail, index = numElems;
                     currentPos != head && index > idx; 
                     currentPos = currentPos.prev, --index)
                    continue;
            }
            else
            {
                for (currentPos = head.next, index = 0;
                     currentPos != tail && index < idx; 
                     currentPos = currentPos.next, ++index)
                    continue;
            }

            synchronized (SimpleList.this)
            {
                next = firstIter;
                firstIter = this;
            }
        }

        public void finalize()
        {
            // Unchain this iterator so we don't waste any more time on it

            synchronized (SimpleList.this)
            {
                SimpleListIterator prev = null;

                for (SimpleListIterator iter = firstIter; iter != null;
                     iter = iter.next)
                {
                    if (iter == this)
                    {
                        if (prev == null)
                            firstIter = next;
                        else
                            prev.next = next;
                        break;
                    }
                }
            }
        }

        /**
         * Inserts the specified element into the List.
         */
        public void add( Object o )
        {
            check();
            firstIter.dirty( this );
            
            Node newNode = new Node();

            newNode.item = o;
            newNode.next = currentPos;
            newNode.prev = currentPos.prev;
            currentPos.prev = newNode;
            newNode.prev.next = newNode;
            ++numElems;
            currentPos = newNode;
            forward = false;
        }

        /**
         * Returns true if this ListIterator has more elements when
         * traversing the List in the forward direction. 
         */
        public boolean hasNext()
        {
            check();

            return (index < numElems);
        }

        /**
         * Returns true if this ListIterator has more elements when
         * traversing the list in the reverse direction. 
         */
        public boolean hasPrevious()
        {
            check();

            return (index != 0);
        }
    
        /**
         * Returns the next element in the List. 
         */
        public Object next()
        {
            check();

            Object retValue = currentPos.item;

            currentPos = currentPos.next;
            ++index;
            forward = true;

            return retValue;
        }
    
        /**
         * Returns the index of the element that would be returned by a
         * subsequent call to next.  
         */
        public int nextIndex()
        {
            check();

            return index;
        }
    
        /**
         * Returns the previous element in the List. 
         */
        public Object previous()
        {
            check();

            currentPos = currentPos.prev;
            --index;
            forward = false;

            return currentPos.item;
        }

        /**
         * Returns the index of the element that would be returned by a
         * subsequent call to previous. 
         */
        public int previousIndex()
        {
            check();

            return index - 1;
        }

        /** 
         * Removes from the List the last element that was returned by next or
         * previous.  
         */
        public void remove()
        {
            check();
            firstIter.dirty( this );

            Node toRemove;

            if (forward)
            {
                toRemove = currentPos.prev;
                --index;
            }
            else
            {
                toRemove = currentPos;
                currentPos = currentPos.next;
            }

            toRemove.next.prev = toRemove.prev;
            toRemove.prev.next = toRemove.next;
            --numElems;
        }

        /**
         * Replaces the last element returned by next or previous with the
         * specified element. 
         */
        public void set( Object o )
        {
            check();
            firstIter.dirty( this );

            if (forward)
                currentPos.prev.item = o;
            else
                currentPos.item = o;
        }

        protected void check()
        {
            if (index < 0)
                throw new ConcurrentModificationException();
        }

        protected void dirty( SimpleListIterator inUse )
        {
            synchronized (SimpleList.this)
            {
                for (SimpleListIterator iter = firstIter; iter != null;
                     iter = iter.next)
                {
                    if (iter != inUse)
                        iter.index = -1;
                }
                firstIter = inUse;
                if (inUse != null)
                    inUse.next = null;
            }
        }
    }
}
