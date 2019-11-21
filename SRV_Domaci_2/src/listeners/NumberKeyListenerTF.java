package listeners;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JTextField;

public class NumberKeyListenerTF extends KeyAdapter {
	private JTextField tf = new JTextField();
	
	public NumberKeyListenerTF(JTextField tf) {
		this.tf = tf;
	}

	@Override
	public void keyPressed(KeyEvent arg0) {
		String value = tf.getText();
        int l = value.length();
        if (arg0.getKeyChar() >= '0' && arg0.getKeyChar() <= '9' || arg0.getKeyChar() == '\b') {
        	tf.setEditable(true);
        	//label.setText("");
        } else {
        	tf.setEditable(false);
           //label.setText("* Enter only numeric digits(0-9)");
        }
		
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
	}

}
