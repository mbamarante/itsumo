/*
 * Created on 23 juin 2005
 *
 */
package frodo.interaction.meeting.gui;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 * @author George
 *
 */
public class NewMeetingDialog extends JDialog {

	public static final int OK = 0;

	public static final int CANCEL = 1;

	private int button = -1;

	private JDialog dialog = null;

	private JPanel jPanel = null;

	private JPanel jPanel1 = null;

	private JPanel jPanel2 = null;

	private JPanel jPanel3 = null;

	private JLabel jLabel = null;

	private JTextField jtxLabel = null;

	private JLabel jLabel1 = null;

	private JTextField jtxRangeFrom = null;

	private JLabel jLabel2 = null;

	private JTextField jtxRangeTo = null;

	private JButton jbOk = null;

	private JButton jbCancel = null;

	/**
	 * This method initializes 
	 * 
	 */
	public NewMeetingDialog() {
		super();
		initialize();
		dialog = this;
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setModal(true);
		this.setContentPane(getJPanel());
		this.setTitle("Add new meeting");
		this.setSize(300, 150);
		this.setLocation(400, 200);
		this.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent e) {
				dialog.setVisible(false);
				dialog.dispose();
				button = CANCEL;
			}
		});
	}

	/**
	 * This method initializes jPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJPanel() {
		if (jPanel == null) {
			jPanel = new JPanel();
			jPanel.setLayout(new BoxLayout(jPanel, BoxLayout.Y_AXIS));
			jPanel.add(getJPanel1(), null);
			jPanel.add(getJPanel2(), null);
			jPanel.add(getJPanel3(), null);
		}
		return jPanel;
	}

	/**
	 * This method initializes jPanel1	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJPanel1() {
		if (jPanel1 == null) {
			jLabel = new JLabel();
			jPanel1 = new JPanel();
			jLabel.setText("label:");
			jPanel1.setPreferredSize(new java.awt.Dimension(145, 20));
			jPanel1.add(jLabel, null);
			jPanel1.add(getJtxLabel(), null);
		}
		return jPanel1;
	}

	/**
	 * This method initializes jPanel2	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJPanel2() {
		if (jPanel2 == null) {
			jLabel2 = new JLabel();
			jLabel1 = new JLabel();
			jPanel2 = new JPanel();
			jLabel1.setText("range:");
			jLabel2.setText("to");
			jPanel2.setPreferredSize(new java.awt.Dimension(87, 20));
			jPanel2.add(jLabel1, null);
			jPanel2.add(getJtxRangeFrom(), null);
			jPanel2.add(jLabel2, null);
			jPanel2.add(getJtxRangeTo(), null);
		}
		return jPanel2;
	}

	/**
	 * This method initializes jPanel3	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJPanel3() {
		if (jPanel3 == null) {
			jPanel3 = new JPanel();
			jPanel3.add(getJbOk(), null);
			jPanel3.add(getJbCancel(), null);
		}
		return jPanel3;
	}

	/**
	 * This method initializes jtxLabel	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getJtxLabel() {
		if (jtxLabel == null) {
			jtxLabel = new JTextField("m_");
			jtxLabel.setPreferredSize(new java.awt.Dimension(100, 20));
		}
		return jtxLabel;
	}

	/**
	 * This method initializes jtxRangeFrom	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getJtxRangeFrom() {
		if (jtxRangeFrom == null) {
			jtxRangeFrom = new JTextField("1");
			jtxRangeFrom.setPreferredSize(new java.awt.Dimension(20, 20));
		}
		return jtxRangeFrom;
	}

	/**
	 * This method initializes jtxRangeTo	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getJtxRangeTo() {
		if (jtxRangeTo == null) {
			jtxRangeTo = new JTextField("8");
			jtxRangeTo.setPreferredSize(new java.awt.Dimension(20, 20));
		}
		return jtxRangeTo;
	}

	/**
	 * This method initializes jbOk	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJbOk() {
		if (jbOk == null) {
			jbOk = new JButton();
			jbOk.setText("Ok");
			jbOk.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					dialog.setVisible(false);
					dialog.dispose();
					button = OK;
				}
			});
		}
		return jbOk;
	}

	/**
	 * This method initializes jbCancel	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJbCancel() {
		if (jbCancel == null) {
			jbCancel = new JButton();
			jbCancel.setText("Cancel");
			jbCancel.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					dialog.setVisible(false);
					dialog.dispose();
					button = CANCEL;
				}
			});
		}
		return jbCancel;
	}

	public int getButton() {
		return button;
	}

	class MeetingOptions {
		private String label = null;

		private int rangeFrom;

		private int rangeTo;

		public String getLabel() {
			return label;
		}

		public void setLabel(String label) {
			this.label = label;
		}

		public int getRangeFrom() {
			return rangeFrom;
		}

		public void setRangeFrom(int rangeFrom) {
			this.rangeFrom = rangeFrom;
		}

		public int getRangeTo() {
			return rangeTo;
		}

		public void setRangeTo(int rangeTo) {
			this.rangeTo = rangeTo;
		}
	}

	public static MeetingOptions showDialod() {
		NewMeetingDialog dialog = new NewMeetingDialog();
		dialog.setVisible(true);
		MeetingOptions options = null;
		if (dialog.getButton() == NewMeetingDialog.OK) {
			options = dialog.new MeetingOptions();
			options.setLabel(dialog.getJtxLabel().getText());
			options.setRangeFrom(Integer.parseInt(dialog.getJtxRangeFrom().getText()));
			options.setRangeTo(Integer.parseInt(dialog.getJtxRangeTo().getText()));
		}
		return options;
	}

	public static void main(String[] args) {
		MeetingOptions options = NewMeetingDialog.showDialod();
		if (options != null) {
			System.out.println(options.getLabel());
		}
	}
} //  @jve:decl-index=0:visual-constraint="10,10"
