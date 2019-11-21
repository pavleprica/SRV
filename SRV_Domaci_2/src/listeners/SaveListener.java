package listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import javax.swing.JOptionPane;

import org.apache.commons.io.FileUtils;
import view.frames.AppFrame;

public class SaveListener implements ActionListener {

	@Override
	public void actionPerformed(ActionEvent e) {
		if (AppFrame.getInstance().getTfFilePath().getText().isEmpty()) {
			JOptionPane.showMessageDialog(AppFrame.getInstance(), "File path cannot be empty!", "Error", JOptionPane.ERROR_MESSAGE);
		} else {
			File file = new File(AppFrame.getInstance().getTfFilePath().getText());
			if (!file.exists()) {
				try {
					FileUtils.touch(file);
//					file.createNewFile();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
			
			StringBuilder sb = new StringBuilder();
			
			if (AppFrame.getInstance().getCbA().isSelected()) {
				sb.append("a|");
			} 
			if (AppFrame.getInstance().getCbAE().isSelected()) {
				sb.append("a + E|");
			} 
			if (AppFrame.getInstance().getCbC().isSelected()) {
				sb.append("C|");
			} 
			if (AppFrame.getInstance().getCbCE().isSelected()) {
				sb.append("C + E|");
			} 
			if (AppFrame.getInstance().getCbD().isSelected()) {
				sb.append("d|");
			} 
			if (AppFrame.getInstance().getCbDE().isSelected()) {
				sb.append("d + E|");
			} 
			if (AppFrame.getInstance().getCbDWC().isSelected()) {
				sb.append("d + W * C|");
			} 
			if (AppFrame.getInstance().getCbDWCE().isSelected()) {
				sb.append("d + W * C + E|");
			} 
			if (AppFrame.getInstance().getCbTest().isSelected()) {
				sb.append("Test|");
			} 
			if (AppFrame.getInstance().getCbTestE().isSelected()) {
				sb.append("Test + E|");
			} 
			if (AppFrame.getInstance().getCbDWTest().isSelected()) {
				sb.append("d + W * Test|");
			} 
			if (AppFrame.getInstance().getCbDWTestE().isSelected()) {
				sb.append("d + W * Test + E|");
			}
			
			if (sb.length() == 0) {
				JOptionPane.showMessageDialog(AppFrame.getInstance(), "You have to select at least one heuristic!", "Error", JOptionPane.ERROR_MESSAGE);
			} else if (AppFrame.getInstance().getTblJobs().getRowCount() == 0) {
				JOptionPane.showMessageDialog(AppFrame.getInstance(), "You have to add at least one job!", "Error", JOptionPane.ERROR_MESSAGE);
			} else {
				try {
					FileWriter fw = new FileWriter(file);
					PrintWriter pw = new PrintWriter(fw);
					
					sb.deleteCharAt(sb.length() - 1);
					pw.println(sb.toString());
					
					for (int i = 0; i < AppFrame.getInstance().getJobs().size(); i++) {
						pw.println(AppFrame.getInstance().getJobs().get(i).toString());	
					}
					
					pw.close();
					fw.close();
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(AppFrame.getInstance(), "Error while trying to write to file!", "Error", JOptionPane.ERROR_MESSAGE);
				}
			}
		}
	}

}
