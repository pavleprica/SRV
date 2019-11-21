package listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.table.DefaultTableModel;

import model.Resource;
import view.frames.AppFrame;

public class DeleteResourceListener implements ActionListener {

	@Override
	public void actionPerformed(ActionEvent e) {
		int selectedRow = AppFrame.getInstance().getTblResources().getSelectedRow();
		if (selectedRow > -1) {
			Resource r = new Resource((String)AppFrame.getInstance().getTblResources().getModel().getValueAt(selectedRow, 0), Integer.parseInt((String)AppFrame.getInstance().getTblResources().getModel().getValueAt(selectedRow, 1)));
			AppFrame.getInstance().getResources().remove(r);
			((DefaultTableModel)AppFrame.getInstance().getTblResources().getModel()).removeRow(AppFrame.getInstance().getTblResources().getSelectedRow());
		}
		
	}

}
