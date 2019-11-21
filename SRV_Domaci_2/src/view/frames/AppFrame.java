package view.frames;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Properties;

import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableModel;

import com.jtattoo.plaf.fast.FastLookAndFeel;

import listeners.DeleteJobListener;
import listeners.DeleteResourceListener;
import listeners.SaveListener;
import listeners.SimulateListener;
import model.Job;
import model.Resource;
import net.miginfocom.swing.MigLayout;
import terminal.TerminalOutput;

import javax.swing.border.BevelBorder;
import javax.swing.border.EmptyBorder;

public class AppFrame extends JFrame {
	private static AppFrame instance = null;
	
	private JButton btnAddResource;
	private JButton btnDeleteResource;
	private JTable tblResources;

	private JButton btnAddJob;
	private JButton btnDeleteJob;
	private JTable tblJobs;

	private JCheckBox cbA;
	private JCheckBox cbAE;
	private JCheckBox cbC;
	private JCheckBox cbCE;
	private JCheckBox cbD;
	private JCheckBox cbDE;
	private JCheckBox cbDWC;
	private JCheckBox cbDWCE;
	private JCheckBox cbTest;
	private JCheckBox cbTestE;
	private JCheckBox cbDWTest;
	private JCheckBox cbDWTestE;
	
	private JTextField tfFilePath;
	private JButton btnSave;
	private JButton btnSimulate;
	
	private ArrayList<Resource> resources = new ArrayList<Resource>();
	private ArrayList<Job> jobs = new ArrayList<Job>();
	private JLabel lblNewLabel_1;

	private AppFrame() {
		setResizable(false);
		initialize();
		addToFrame();
	}

	private void initialize() {
		try {
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
		} catch (Exception e) {
			e.printStackTrace();
		}
		setTitle("SRV");
		setLocationRelativeTo(null);
		setBounds(100, 100, 600, 900);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		getContentPane().setLayout(new MigLayout("", "[][][][][][pref!,fill][fill]", "[20px:20px:20px][120px:120px:120px][20px:20px:20px][170px:170px:170px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px][20px:20px:20px]"));
		
		btnAddResource = new JButton("Add Resource");
		btnAddResource.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				AddResourceFrame frame = new AddResourceFrame();
				frame.setVisible(true);
			}
		});
		
		btnDeleteResource = new JButton("Delete Resource");
		btnDeleteResource.addActionListener(new DeleteResourceListener());
		
		tblResources = new JTable();
		tblResources.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		tblResources.setModel(new DefaultTableModel(
			new Object[][] {
			},
			new String[] {
				"Name", "Eearliest Available Time"
			}
		) {
			boolean[] columnEditables = new boolean[] {
				false, true
			};
			public boolean isCellEditable(int row, int column) {
				return columnEditables[column];
			}
		});
		
		btnAddJob = new JButton("Add Job");
		btnAddJob.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				AddJobFrame frame = new AddJobFrame();
				frame.setVisible(true);
			}
		});
		
		btnDeleteJob = new JButton("Delete Job");
		btnDeleteJob.addActionListener(new DeleteJobListener());
		
		tblJobs = new JTable();
		tblJobs.setEnabled(false);
		tblJobs.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		tblJobs.setBorder(new EmptyBorder(0, 0, 0, 0));
		tblJobs.setModel(new DefaultTableModel(
				new Object[][] {
				},
				new String[] {
					"Name", "Start Time", "Job Time", "Deadline", "Function", "Resources"
				}
			) {
				boolean[] columnEditables = new boolean[] {
					false, true
				};
				public boolean isCellEditable(int row, int column) {
					return columnEditables[column];
				}
			});
		
		cbA = new JCheckBox("a");
		cbAE = new JCheckBox("a + E");
		cbC = new JCheckBox("C");
		cbCE = new JCheckBox("C + E");
		cbD = new JCheckBox("d");
		cbDE = new JCheckBox("d + E");
		cbDWC = new JCheckBox("d + W * C");
		cbDWCE = new JCheckBox("d + W * C + E");
		
		cbTest = new JCheckBox("Test");
		cbTestE = new JCheckBox("Test + E");
		cbDWTest = new JCheckBox("d + W * Test");
		cbDWTestE = new JCheckBox("d + W * Test + E");
		
		tfFilePath = new JTextField();
		
		btnSave = new JButton("Save");
		btnSave.addActionListener(new SaveListener());
		btnSimulate = new JButton("Simulate");
		btnSimulate.addActionListener(new SimulateListener());
		
	}
	
	private void addToFrame() {
		
		JLabel lblResources = new JLabel("Resources:");
		getContentPane().add(lblResources, "cell 0 0,alignx left");
		
		getContentPane().add(btnAddResource, "cell 6 0,alignx right");
		getContentPane().add(btnDeleteResource, "cell 5 0,alignx right");
		getContentPane().add(new JScrollPane(tblResources), "cell 0 1 7 1,grow");
		
		JLabel lblJobs = new JLabel("Jobs:");
		getContentPane().add(lblJobs, "cell 0 2,alignx left");
		
		getContentPane().add(btnAddJob, "cell 6 2,alignx right");
		//getContentPane().add(btnDeleteJob, "cell 5 2,alignx right");
		getContentPane().add(new JScrollPane(tblJobs), "cell 0 3 7 1,grow");

		JLabel lblHeuristics = new JLabel("Heuristics:");
		getContentPane().add(lblHeuristics, "cell 0 5,alignx center");
		
		JLabel lblBasic = new JLabel("Basic:");
		lblBasic.setToolTipText("");
		getContentPane().add(lblBasic, "cell 2 5,alignx left");
		
		getContentPane().add(cbA, "cell 2 6");
		getContentPane().add(cbAE, "cell 3 6");
		getContentPane().add(cbC, "cell 2 7");
		getContentPane().add(cbCE, "cell 3 7");
		getContentPane().add(cbD, "cell 2 8");
		getContentPane().add(cbDE, "cell 3 8");
		getContentPane().add(cbDWC, "cell 2 9");
		getContentPane().add(cbDWCE, "cell 3 9");
		
		JLabel lblUsingResources = new JLabel("Using Resources:");
		getContentPane().add(lblUsingResources, "cell 5 5 2 1,alignx left");
		
		getContentPane().add(cbTest, "cell 5 6");
		getContentPane().add(cbTestE, "cell 6 6");
		getContentPane().add(cbDWTest, "cell 5 7");
		getContentPane().add(cbDWTestE, "cell 6 7");
		
		lblNewLabel_1 = new JLabel("   ");
		lblNewLabel_1.setToolTipText("N.S.");
		getContentPane().add(lblNewLabel_1, "cell 6 9");

		JLabel lblNewLabel = new JLabel("File path:");
		getContentPane().add(lblNewLabel, "cell 0 11,alignx left");
		
		getContentPane().add(tfFilePath, "cell 1 11 4 1,growx");
		tfFilePath.setColumns(10);
		
		getContentPane().add(btnSave, "cell 5 11");
		getContentPane().add(btnSimulate, "cell 6 11");
		getContentPane().add(new JLabel("Terminal output:"), "cell 1 12");

		JScrollPane terminalPane = new JScrollPane(TerminalOutput.getInstance());
		setCmpSize(terminalPane, 400, 250);
		getContentPane().add(terminalPane, "cell 1 13 8 25,grow");
	}

	public JButton getBtnAddResource() {
		return btnAddResource;
	}

	public JButton getBtnDeleteResource() {
		return btnDeleteResource;
	}

	public JTable getTblResources() {
		return tblResources;
	}

	public JButton getBtnAddJob() {
		return btnAddJob;
	}

	public JButton getBtnDeleteJob() {
		return btnDeleteJob;
	}

	public JTable getTblJobs() {
		return tblJobs;
	}

	public JCheckBox getCbA() {
		return cbA;
	}

	public JCheckBox getCbAE() {
		return cbAE;
	}

	public JCheckBox getCbC() {
		return cbC;
	}

	public JCheckBox getCbCE() {
		return cbCE;
	}

	public JCheckBox getCbD() {
		return cbD;
	}

	public JCheckBox getCbDE() {
		return cbDE;
	}

	public JCheckBox getCbDWC() {
		return cbDWC;
	}

	public JCheckBox getCbDWCE() {
		return cbDWCE;
	}

	public JCheckBox getCbTest() {
		return cbTest;
	}

	public JCheckBox getCbTestE() {
		return cbTestE;
	}

	public JCheckBox getCbDWTest() {
		return cbDWTest;
	}

	public JCheckBox getCbDWTestE() {
		return cbDWTestE;
	}

	public JTextField getTfFilePath() {
		return tfFilePath;
	}

	public JButton getBtnSave() {
		return btnSave;
	}

	public JButton getBtnSimulate() {
		return btnSimulate;
	}
	
	public static AppFrame getInstance() {
		if (instance == null) instance = new AppFrame();
		return instance;
	}
	
	public ArrayList<Resource> getResources() {
		return resources;
	}
	
	public ArrayList<Job> getJobs() {
		return jobs;
	}

	private void setCmpSize(JComponent component, int width, int height) {
		component.setMinimumSize(new Dimension(width, height));
		component.setPreferredSize(new Dimension(width, height));
		component.setMaximumSize(new Dimension(width, height));
	}

}
