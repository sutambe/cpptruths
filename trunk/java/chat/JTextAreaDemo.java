import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class JTextAreaDemo extends JFrame implements ActionListener {
	JTextField jtfInput;

	JTextArea jtAreaOutput;

	String newline = "\n";

	public JTextAreaDemo() {
		createGui();
	}

	public void createGui() {
		jtfInput = new JTextField(20);
		jtfInput.addActionListener(this);

		jtAreaOutput = new JTextArea(5, 20);
		jtAreaOutput.setCaretPosition(jtAreaOutput.getDocument().getLength());
		jtAreaOutput.setEditable(false);
		JScrollPane scrollPane = new JScrollPane(jtAreaOutput,
				JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		
		
		GridBagLayout gridBag = new GridBagLayout();
		Container contentPane = getContentPane();
		contentPane.setLayout(gridBag);
		
		GridBagConstraints gridCons1 = new GridBagConstraints();
       		gridCons1.gridwidth = GridBagConstraints.REMAINDER;
        	gridCons1.fill = GridBagConstraints.HORIZONTAL;
        	contentPane.add(jtfInput, gridCons1);
        
        	GridBagConstraints gridCons2 = new GridBagConstraints();
        	gridCons2.weightx = 1.0;
        	gridCons2.weighty = 1.0;
        	contentPane.add(scrollPane, gridCons2);

	}

	public void actionPerformed(ActionEvent evt) {
		String text = jtfInput.getText();
		jtAreaOutput.append(text + newline);
		jtfInput.selectAll();
	}
	
	   public static void main(String[] args) {
		   JTextAreaDemo jtfTfDemo = new JTextAreaDemo();		
	        jtfTfDemo.pack();
	        jtfTfDemo.addWindowListener(new WindowAdapter() {
	            public void windowClosing(WindowEvent e) {
	                System.exit(0);
	            }
	        });
	        jtfTfDemo.setVisible(true);
	    }
}
