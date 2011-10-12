package adopt.common;

import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Calendar;

public class Clock {

    GregorianCalendar gc = new GregorianCalendar();

    private int time () {
	return (gc.get(Calendar.SECOND) + gc.get(Calendar.MINUTE)*60 + 
		gc.get(Calendar.HOUR)*60*60);
    }

    public void printTime () {
    	System.out.println(time());
    }

    public int getTime () {
	return time();
    }

    public static void main (String argv[]) {
	
	(new Clock()).printTime();

    }

}
