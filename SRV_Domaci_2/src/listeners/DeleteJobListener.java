package listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.table.DefaultTableModel;

import model.Job;
import model.Resource;
import view.frames.AppFrame;

public class DeleteJobListener implements ActionListener {

	@Override
	public void actionPerformed(ActionEvent e) {
		int selectedRow = AppFrame.getInstance().getTblJobs().getSelectedRow();
		if (selectedRow > -1) {
			Job j = new Job((String)AppFrame.getInstance().getTblJobs().getModel().getValueAt(selectedRow, 0), 0, 0, 0, "", null);
			AppFrame.getInstance().getJobs().remove(j);
			((DefaultTableModel)AppFrame.getInstance().getTblJobs().getModel()).removeRow(AppFrame.getInstance().getTblResources().getSelectedRow());
		}
		
	}

}
