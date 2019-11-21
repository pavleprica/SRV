package listeners;

import terminal.TerminalOutput;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.lang.ProcessBuilder.Redirect;

public class SimulateListener implements ActionListener {

	@Override
	public void actionPerformed(ActionEvent e) {
		try {
			String dirPath = "C:\\Users\\pavle\\eclipse-workspace\\FreeRTOS5-izvrsavanje\\Debug";
			String filePath = "C:/Users/pavle/eclipse-workspace/FreeRTOS5-izvrsavanje/Debug/FreeRTOS5-izvrsavanje.exe";
			File f = new File(filePath);
			ProcessBuilder builder = new ProcessBuilder(filePath);
			builder.directory(new File(dirPath));
			builder.redirectOutput(Redirect.PIPE);
			Process proces = builder.start();

			try (BufferedReader reader = new BufferedReader(
	                new InputStreamReader(proces.getInputStream()))) {

	            String line;
	            while ((line = reader.readLine()) != null) {
	            	if(line.contains("terminate")) break;
					TerminalOutput.getInstance().putText(line);
					SwingUtilities.updateComponentTreeUI(TerminalOutput.getInstance());
	            }
			}
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
	}

}
