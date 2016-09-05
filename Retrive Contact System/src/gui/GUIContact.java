package gui;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JButton;

import task.Contact;

public class GUIContact extends JFrame{
	public JTextField txtName;
	public JTextField txtPhone;
	public JTextField txtGroup;
	public JTextField txtGroupRg;
	Contact contact;
	
	public GUIContact(Contact contact) {
		this.contact = contact;
		
		setResizable(false);
		setTitle("\uC5F0\uB77D\uCC98");
		getContentPane().setLayout(null);
		
		JLabel lblNewLabel = new JLabel("\uC774      \uB984:");
		lblNewLabel.setBounds(10, 10, 62, 25);
		getContentPane().add(lblNewLabel);
		
		JLabel label = new JLabel("\uC804\uD654\uBC88\uD638:");
		label.setBounds(10, 41, 62, 25);
		getContentPane().add(label);
		
		JLabel label_1 = new JLabel("\uADF8 \uB8F9 \uBA85 :");
		label_1.setBounds(10, 72, 62, 25);
		getContentPane().add(label_1);
		
		txtName = new JTextField();
		txtName.setBounds(78, 10, 153, 25);
		getContentPane().add(txtName);
		txtName.setColumns(10);
		if(contact != null)
			txtName.setText(contact.name);
		
		txtPhone = new JTextField();
		txtPhone.setColumns(10);
		txtPhone.setBounds(78, 41, 153, 25);
		getContentPane().add(txtPhone);
		if(contact != null)
			txtPhone.setText(contact.tel);
		
		txtGroup = new JTextField();
		txtGroup.setColumns(10);
		txtGroup.setBounds(78, 72, 153, 25);
		getContentPane().add(txtGroup);
		if(contact != null)
			txtGroup.setText(contact.group);
		
		JButton btnNewButton = new JButton();
		if(contact == null)
			btnNewButton.setText("연락처등록");
		else
			btnNewButton.setText("연락처수정");
		
		btnNewButton.setBounds(10, 129, 221, 25);
		getContentPane().add(btnNewButton);
		
		JLabel label_2 = new JLabel("\uADF8 \uB8F9 \uBA85 :");
		label_2.setBounds(252, 41, 62, 25);
		getContentPane().add(label_2);
		
		txtGroupRg = new JTextField();
		txtGroupRg.setColumns(10);
		txtGroupRg.setBounds(320, 41, 153, 25);
		getContentPane().add(txtGroupRg);
		
		JButton btnNewButton_1 = new JButton("\uADF8\uB8F9\uBA85 \uB4F1\uB85D");
		btnNewButton_1.setBounds(252, 88, 97, 23);
		getContentPane().add(btnNewButton_1);
		
		JButton button = new JButton("\uADF8\uB8F9\uBA85 \uC0AD\uC81C");
		button.setBounds(376, 88, 97, 23);
		getContentPane().add(button);
	}
}
