/*
 * Created on 2 mai 2005
 *
 */
package frodo.monitoring.interfaces;

/**
 * @author gushakov
 *
 */
public interface MonitorListener {
	public void updateStatistics(Object[] parameterValue);

	public void setUp();

	public void displayMessage(String message);
}
