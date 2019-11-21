package listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;

import model.Resource;
import view.frames.AddResourceFrame;
import view.frames.AppFrame;

public class AddResourceListener implements ActionListener {

	private AddResourceFrame addResourceFrame;
	
	public AddResourceListener(AddResourceFrame addResourceFrame) {
		this.addResourceFrame = addResourceFrame;
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if (!addResourceFrame.getTfResourceName().getText().isEmpty() && !addResourceFrame.getTfEAT().getText().isEmpty()) {
			Resource r = new Resource(addResourceFrame.getTfResourceName().getText(), Integer.parseInt(addResourceFrame.getTfEAT().getText()));
			if (!AppFrame.getInstance().getResources().contains(r)) {
				addResourceFrame.dispose();
				AppFrame.getInstance().getResources().add(r);
				((DefaultTableModel)AppFrame.getInstance().getTblResources().getModel()).addRow(new Object[] {addResourceFrame.getTfResourceName().getText(), addResourceFrame.getTfEAT().getText()});
			} else {
				JOptionPane.showMessageDialog(AppFrame.getInstance(), "Resource with entered name already exists.", "Error", JOptionPane.ERROR_MESSAGE);
			}
		} else {
			JOptionPane.showMessageDialog(AppFrame.getInstance(), "All fields must be entered.", "Error", JOptionPane.ERROR_MESSAGE);
		}
	}

}
