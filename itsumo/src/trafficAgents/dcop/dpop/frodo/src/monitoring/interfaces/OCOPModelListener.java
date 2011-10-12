/*
 * Created on Jul 8, 2005
 */
package frodo.monitoring.interfaces;

/**
 * @author ushakov
 */
public interface OCOPModelListener {
	public void ocopModelChanged(OCOPModelEvent event);

	public void highlightChanged(OCOPSelectionEvent event);

}
