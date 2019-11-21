package view.frames;

import java.awt.TrayIcon.MessageType;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;

import listeners.AddResourceListener;
import listeners.NumberKeyListenerTF;
import model.Resource;
import net.miginfocom.swing.MigLayout;

public class AddResourceFrame extends JFrame {

	private JPanel contentPane;
	private JTextField tfResourceName;
	private JTextField tfEAT;
	private JButton btnAddResource;

	/**
	 * Create the frame.
	 */
	public AddResourceFrame() {
		initialize();
		addToFrame();
	}
	
	private void initialize() {
		setTitle("Add Resource");
		setLocationRelativeTo(null);
		setResizable(false);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setBounds(100, 100, 250, 170);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(new MigLayout("", "[][grow]", "[20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px]"));
		
		tfResourceName = new JTextField();
		tfResourceName.setColumns(10);
		
		tfEAT = new JTextField();
		tfEAT.addKeyListener(new NumberKeyListenerTF(tfEAT));
		tfEAT.setToolTipText("Earliest Available Time");
		tfEAT.setColumns(10);
		
		btnAddResource = new JButton("Add resource");
		btnAddResource.addActionListener(new AddResourceListener(this));
	}
	
	private void addToFrame() {
		JLabel lblResourceName = new JLabel("Resource name:");
		contentPane.add(lblResourceName, "cell 0 1,alignx trailing");
		
		contentPane.add(tfResourceName, "cell 1 1,growx");
		
		JLabel lblEat = new JLabel("EAT:");
		lblEat.setToolTipText("Earliest Available Time");
		contentPane.add(lblEat, "cell 0 2,alignx trailing");
		
		contentPane.add(tfEAT, "cell 1 2,growx");
		
		contentPane.add(btnAddResource, "cell 0 4 2 1,alignx center");
	}

	public JTextField getTfResourceName() {
		return tfResourceName;
	}
	
	public JTextField getTfEAT() {
		return tfEAT;
	}
}
