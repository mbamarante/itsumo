package frodo.MeetingScheduling.Hierarchical;

import java.util.Vector;

public class Department extends Object{

     public String name;
     public int id;
     public Vector meetings; // the meeting index
     public Vector utilities;

    public Department() {
        meetings = new Vector();
        utilities = new Vector();
    }
}
