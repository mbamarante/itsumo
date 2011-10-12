/*
 * Created on 2 mai 2005
 *
 */
package frodo.monitoring.gui;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

import frodo.monitoring.interfaces.MonitorListener;


/**
 * @author gushakov
 *
 */
public class MonitorWindow extends JFrame implements MonitorListener {

	private JPanel jPanel = null;

	private JTable jTable = null;

	private JScrollPane jScrollPane = null;

	private JTabbedPane jTabbedPane = null;

	private DefaultTableModel tableModel = null;

	/**
	 * This is the default constructor
	 */
	public MonitorWindow() {
		super();
		initialize();

		String[] columnNames = { "Parameter", "Value" };
		tableModel = (DefaultTableModel) jTable.getModel();
		tableModel.setColumnIdentifiers(columnNames);

	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setTitle("Problem Monitor");
		this.setContentPane(getJTabbedPane());
		this.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent e) {
				System.out.println("closing statistics monitor");
				closeWindow();
			}
		});
	}

	private void closeWindow() {
		this.setVisible(false);
		this.dispose();
	}

	/**
	 * This method initializes jPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJPanel() {
		if (jPanel == null) {
			jPanel = new JPanel();
			jPanel.add(getJScrollPane(), null);
		}
		return jPanel;
	}

	/**
	 * This method initializes jTable	
	 * 	
	 * @return javax.swing.JTable	
	 */
	private JTable getJTable() {
		if (jTable == null) {
			jTable = new JTable();
			jTable.setModel(new DefaultTableModel());
		}
		return jTable;
	}

	/**
	 * This method initializes jScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getJScrollPane() {
		if (jScrollPane == null) {
			jScrollPane = new JScrollPane();
			jScrollPane.setViewportView(getJTable());
			jScrollPane.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		}
		return jScrollPane;
	}

	/**
	 * This method initializes jTabbedPane	
	 * 	
	 * @return javax.swing.JTabbedPane	
	 */
	private JTabbedPane getJTabbedPane() {
		if (jTabbedPane == null) {
			jTabbedPane = new JTabbedPane();
			jTabbedPane.addTab("Statistics", null, getJPanel(), null);
		}
		return jTabbedPane;
	}

	public void setUp() {
		// The monitor window is set visible from the meeting explorer window.
		this.pack();
		this.setVisible(true);
	}

	public void updateStatistics(Object[] parameterValue) {
		tableModel.addRow(parameterValue);
	}

	public void displayMessage(String message) {
	}

	public static void main(String[] args) {
		MonitorWindow win = new MonitorWindow();
		win.pack();
		win.setVisible(true);
	}

} //  @jve:decl-index=0:visual-constraint="10,10"
