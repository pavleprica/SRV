package view.frames;

import java.awt.Dialog.ModalExclusionType;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

import listeners.AddJobListener;
import listeners.NumberKeyListenerTF;
import model.Job;
import model.Resource;
import net.miginfocom.swing.MigLayout;
import view.checkedComboBox.CheckableItem;
import view.checkedComboBox.CheckedComboBox;

public class AddJobFrame extends JFrame {

	private JPanel contentPane;
	private JTextField tfJobName;
	private JTextField tfStartTime;
	private JTextField tfJobTime;
	private JTextField tfDeadline;
	
	private CheckedComboBox<CheckableItem> ccbJobResources;
	private JComboBox cbJobFunction;
	
	/**
	 * Create the frame.
	 */
	public AddJobFrame() {
		setResizable(false);
		setModalExclusionType(ModalExclusionType.TOOLKIT_EXCLUDE);
		setTitle("Add Job");
		setLocationRelativeTo(null);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setBounds(100, 100, 260, 245);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(new MigLayout("", "[][140px:140px:140px,grow]", "[20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][]"));
		
		JLabel lblJobName = new JLabel("Job Name:");
		contentPane.add(lblJobName, "cell 0 0,alignx trailing");
		
		tfJobName = new JTextField();
		contentPane.add(tfJobName, "cell 1 0,growx");
		tfJobName.setColumns(10);
		
		JLabel lblStartTime = new JLabel("Start time:");
		contentPane.add(lblStartTime, "cell 0 1,alignx trailing");
		
		tfStartTime = new JTextField();
		tfStartTime.addKeyListener(new NumberKeyListenerTF(tfStartTime));
		contentPane.add(tfStartTime, "cell 1 1,growx");
		tfStartTime.setColumns(10);
		
		JLabel lblJobTime = new JLabel("Job time:");
		contentPane.add(lblJobTime, "cell 0 2,alignx trailing");
		
		tfJobTime = new JTextField();
		tfJobTime.addKeyListener(new NumberKeyListenerTF(tfJobTime));
		contentPane.add(tfJobTime, "cell 1 2,growx");
		tfJobTime.setColumns(10);
		
		JLabel lblDeadline = new JLabel("Deadline:");
		contentPane.add(lblDeadline, "cell 0 3,alignx trailing");
		
		tfDeadline = new JTextField();
		tfDeadline.addKeyListener(new NumberKeyListenerTF(tfDeadline));
		contentPane.add(tfDeadline, "cell 1 3,growx");
		tfDeadline.setColumns(10);
		
		JLabel lblJobFunction = new JLabel("Job function:");
		contentPane.add(lblJobFunction, "cell 0 4,alignx trailing");
		
		cbJobFunction = new JComboBox();
		cbJobFunction.setModel(new DefaultComboBoxModel(new String[] {".", ",", ":", "!", "@", "#", "%", "^", "&", "*", "(", ")", "-", "+"}));
		contentPane.add(cbJobFunction, "cell 1 4,growx");
		
		JLabel lblJobResources = new JLabel("Job resources:");
		contentPane.add(lblJobResources, "cell 0 5,alignx trailing");
		
 		CheckableItem[] items = new CheckableItem[AppFrame.getInstance().getResources().size()];
 		for (int  i = 0; i < AppFrame.getInstance().getResources().size(); i++) {
 			items[i] = new CheckableItem(AppFrame.getInstance().getResources().get(i).toString(), false);
 		}
		ccbJobResources = new CheckedComboBox<>(new DefaultComboBoxModel<>(items));
		contentPane.add(ccbJobResources, "cell 1 5,growx");
		
		JButton btnAddJob = new JButton("Add Job");
		btnAddJob.addActionListener(new AddJobListener(this));
		contentPane.add(btnAddJob, "cell 0 7 2 1,alignx center");
	}

	public JPanel getContentPane() {
		return contentPane;
	}

	public JTextField getTfJobName() {
		return tfJobName;
	}

	public JTextField getTfStartTime() {
		return tfStartTime;
	}

	public JTextField getTfJobTime() {
		return tfJobTime;
	}

	public JTextField getTfDeadline() {
		return tfDeadline;
	}

	public CheckedComboBox<CheckableItem> getCcbJobResources() {
		return ccbJobResources;
	}
	
	public JComboBox getCbJobFunction() {
		return cbJobFunction;
	}
}
