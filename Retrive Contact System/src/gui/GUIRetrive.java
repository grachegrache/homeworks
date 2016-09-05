package gui;

import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;
import javax.swing.JTextArea;

import task.Contact;

public class GUIRetrive extends JFrame{
	public JTextField txtInput;
	public JTable table;
	public JTextArea txtArea;
	public ArrayList<Contact> retrivedList;
	public int type;
	
	public GUIRetrive(int type) {
		this.type = type;
		
		retrivedList = new ArrayList<Contact>();
		setResizable(false);
		setTitle("\uAC80\uC0C9");
		getContentPane().setLayout(null);
		
		txtInput = new JTextField();
		txtInput.setBounds(10, 10, 202, 21);
		getContentPane().add(txtInput);
		txtInput.setColumns(10);
		
		JButton btnNewButton = new JButton("\uAC80\uC0C9");
		btnNewButton.setBounds(224, 9, 97, 23);
		getContentPane().add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("\uAC80\uC0C9\uCDE8\uC18C");
		btnNewButton_1.setBounds(333, 9, 97, 23);
		getContentPane().add(btnNewButton_1);
		
		JButton btnNewButton_2 = new JButton("\uC5F0\uB77D\uCC98 \uC218\uC815");
		btnNewButton_2.setBounds(10, 244, 119, 23);
		getContentPane().add(btnNewButton_2);
		
		JButton btnNewButton_3 = new JButton("\uC5F0\uB77D\uCC98 \uC0AD\uC81C");
		btnNewButton_3.setBounds(141, 244, 114, 23);
		getContentPane().add(btnNewButton_3);
		
		JButton btnNewButton_4 = new JButton("SMS \uC804\uC1A1");
		btnNewButton_4.setBounds(267, 244, 219, 23);
		getContentPane().add(btnNewButton_4);
		
		table = new JTable();
		table.setModel(new DefaultTableModel(
			new Object[][] {
			},
			new String[] {
				"\uC774\uB984", "\uC804\uD654\uBC88\uD638", "\uADF8\uB8F9\uBA85"
			}
		));
		table.setBounds(12, 57, 244, 177);
		getContentPane().add(table);
		
		txtArea = new JTextArea();
		txtArea.setBounds(267, 57, 215, 177);
		getContentPane().add(txtArea);
	}
}
