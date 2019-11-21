package terminal;

import javax.swing.*;
import java.awt.*;

public class TerminalOutput extends JTextArea {

    private static TerminalOutput instance = null;

    private JTextArea output;


    private TerminalOutput() {
        this.setText("");
    }

    public static TerminalOutput getInstance() {
        if(instance == null) {
            instance = new TerminalOutput();
            instance.init();
        }

        return instance;
    }

    private void init() {
        this.setEditable(false);
        this.setCmpSize(this, 400, 250);
    }

    public void putText(String text) {
        this.setText(this.getText() + text + "\n");
    }

    private void setCmpSize(JComponent component, int width, int height) {
        component.setMinimumSize(new Dimension(width, height));
        component.setPreferredSize(new Dimension(width, height));
        component.setMaximumSize(new Dimension(width, height));
    }

}
