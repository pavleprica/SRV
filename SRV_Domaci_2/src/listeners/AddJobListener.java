package listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;

import model.Job;
import model.Resource;
import view.checkedComboBox.CheckableItem;
import view.frames.AddJobFrame;
import view.frames.AppFrame;

public class AddJobListener implements ActionListener {
	private AddJobFrame addJobFrame;
	
	public AddJobListener(AddJobFrame addJobFrame) {
		this.addJobFrame = addJobFrame;
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if (!addJobFrame.getTfJobName().getText().isEmpty() && !addJobFrame.getTfStartTime().getText().isEmpty() && !addJobFrame.getTfJobTime().getText().isEmpty() && !addJobFrame.getTfDeadline().getText().isEmpty()) {
			ArrayList<Resource> selectedResources = new ArrayList<Resource>();
			for (int i = 0; i < addJobFrame.getCcbJobResources().getModel().getSize(); i++) {
				CheckableItem item = (CheckableItem) addJobFrame.getCcbJobResources().getModel().getElementAt(i);
				if (item.isSelected()) {
					String parse[] = item.toString().split("\\[");
					Resource r = new Resource(parse[0], Integer.parseInt(parse[1].substring(0, parse[1].length() - 1)));
					selectedResources.add(r);
				}
			}
			Job job = new Job(addJobFrame.getTfJobName().getText(), Integer.parseInt(addJobFrame.getTfStartTime().getText()), Integer.parseInt(addJobFrame.getTfJobTime().getText()), Integer.parseInt(addJobFrame.getTfDeadline().getText()), addJobFrame.getCbJobFunction().getSelectedItem().toString(), selectedResources);
		
			if (!AppFrame.getInstance().getJobs().contains(job)) {
				addJobFrame.dispose();
				AppFrame.getInstance().getJobs().add(job);
				((DefaultTableModel)AppFrame.getInstance().getTblJobs().getModel()).addRow(new Object[] {job.getName(), job.getStartTime(), job.getJobTime(), job.getDeadline(), job.getFunction(), job.getResources()});
			} else {
				JOptionPane.showMessageDialog(AppFrame.getInstance(), "Job with entered name already exists.", "Error", JOptionPane.ERROR_MESSAGE);
			}
		} else {
			JOptionPane.showMessageDialog(AppFrame.getInstance(), "All fields must be entered.", "Error", JOptionPane.ERROR_MESSAGE);
		}
	}
}
