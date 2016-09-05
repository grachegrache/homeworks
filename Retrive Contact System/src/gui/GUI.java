package gui;

import javax.swing.JFrame;
import javax.swing.JButton;


public class GUI extends JFrame {
	public GUI() {
		setResizable(false);
		setTitle("\uC5F0\uB77D\uCC98 \uAC80\uC0C9 \uC2DC\uC2A4\uD15C");
		getContentPane().setLayout(null);
		
		JButton btnNewButton = new JButton("\uC5F0\uB77D\uCC98");
		btnNewButton.setBounds(12, 53, 94, 65);
		getContentPane().add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("\uC774\uB984\uAC80\uC0C9");
		btnNewButton_1.setBounds(118, 53, 88, 65);
		getContentPane().add(btnNewButton_1);
		
		JButton btnNewButton_2 = new JButton("\uC804\uD654\uBC88\uD638\uAC80\uC0C9");
		btnNewButton_2.setBounds(218, 53, 105, 65);
		getContentPane().add(btnNewButton_2);
		
		JButton btnNewButton_3 = new JButton("\uADF8\uB8F9\uAC80\uC0C9");
		btnNewButton_3.setBounds(335, 53, 88, 65);
		getContentPane().add(btnNewButton_3);
	}
}
