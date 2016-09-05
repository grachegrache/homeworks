package gui;

import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.Font;

public class GUISMS extends JFrame{
	private final JLabel labelPhone = new JLabel("");
	private final JLabel lblNewLabel_1 = new JLabel("\uAC10\uC0AC\uD569\uB2C8\uB2E4. \uC88B\uC740\uD558\uB8E8\uB418\uC138\uC694~");
	
	public GUISMS(String phone) {
		setResizable(false);
		setTitle("SMS");
		getContentPane().setLayout(null);
		
		JButton btnNewButton = new JButton("\uD655\uC778");
		btnNewButton.setBounds(47, 97, 89, 34);
		getContentPane().add(btnNewButton);
		labelPhone.setFont(new Font("±¼¸²", Font.BOLD, 15));
		labelPhone.setText(phone);
		labelPhone.setBounds(12, 10, 152, 34);
		getContentPane().add(labelPhone);
		lblNewLabel_1.setBounds(12, 54, 213, 23);
		getContentPane().add(lblNewLabel_1);
	}
	
}
