package gui;

import gui.thread.ThreadDelete;
import gui.thread.ThreadRegister;
import gui.thread.ThreadSesarch;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Properties;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import org.jdatepicker.impl.JDatePanelImpl;
import org.jdatepicker.impl.JDatePickerImpl;
import org.jdatepicker.impl.UtilDateModel;

public class MoneyPanel extends JPanel implements ActionListener{
	public JButton			btnDelete;
	public JButton			btnRegister;
	public JButton			btnSearch;
	public JDatePickerImpl	datePicker;
	private JLabel			labelComment;
	private JLabel			labelMoney;
	public JTextArea		text;
	public JTextField		txtComment;
	public JTextField		txtMoney;
	public String			type;
	
	public MoneyPanel(){
		UtilDateModel model = new UtilDateModel();
		Properties pro = new Properties();
		pro.put("text.today", "Today");
		pro.put("text.month", "Month");
		pro.put("text.year", "Year");
		JDatePanelImpl datePanel = new JDatePanelImpl(model, pro);
		
		datePicker = new JDatePickerImpl(datePanel, new DateLabelFormatter());
		txtComment = new JTextField();
		txtMoney = new JTextField();
		btnRegister = new JButton("등록");
		btnSearch = new JButton("검색");
		btnDelete = new JButton("삭제");
		text = new JTextArea();
		
		btnRegister.setPreferredSize(new Dimension(300, 30));
		btnSearch.setPreferredSize(new Dimension(100, 30));
		btnDelete.setPreferredSize(new Dimension(100, 30));
		txtComment.setPreferredSize(new Dimension(100, 30));
		txtMoney.setPreferredSize(new Dimension(100, 30));
		text.setPreferredSize(new Dimension(300, 350));
		
		btnRegister.addActionListener(this);
		btnSearch.addActionListener(this);
		btnDelete.addActionListener(this);
		
		JScrollPane scrollPane = new JScrollPane(text);
		JLabel labelDate = new JLabel("날    짜:");
		labelComment = new JLabel();
		labelMoney = new JLabel();
		
		JPanel p1 = new JPanel();
		// p1.setLayout(new BoxLayout(p1, BoxLayout.Y_AXIS));
		JPanel p2 = new JPanel();
		// p2.setLayout(new BoxLayout(p2, BoxLayout.Y_AXIS));
		setLayout(new GridLayout(1, 2));
		
		JPanel pDate = new JPanel();
		JPanel pComment = new JPanel();
		JPanel pMoney = new JPanel();
		JPanel pBtn = new JPanel();
		// pDate.setBounds(10, 20, WIDTH / 2 - 15, 5);
		// pComment.setBounds(10, 45, WIDTH / 2 - 15, 5);
		// pMoney.setBounds(10, 55, WIDTH / 2 - 15, 5);
		
		// btnRegister.setBounds(10, 80, WIDTH / 2 - 15, 5);
		// pBtn.setBounds(WIDTH / 2 + 5, 20, WIDTH / 2 - 15, 5);
		// text.setBounds(WIDTH / 2 + 5, 30, WIDTH / 2 - 15, HEIGHT - 35);
		
		pDate.add(labelDate);
		pDate.add(datePicker);
		pComment.add(labelComment);
		pComment.add(txtComment);
		pMoney.add(labelMoney);
		pMoney.add(txtMoney);
		pBtn.add(btnSearch);
		pBtn.add(btnDelete);
		
		p1.add(pDate);
		// p1.add(new JComponent(){
		// });
		p1.add(pComment);
		p1.add(pMoney);
		p1.add(btnRegister);
		
		p2.add(pBtn);
		p2.add(scrollPane);
		
		add(p1);
		add(p2);
		
	}
	
	public MoneyPanel(String input){
		this();
		type = input;
		switch(type){
			case "in":
				labelComment.setText("수입내역");
				labelMoney.setText("수입금액");
				break;
			case "out":
				labelComment.setText("지출내역");
				labelMoney.setText("지출금액");
				break;
			default:
				return;
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent e){
		if(GUI.isAccessingExcel == false)
			if(e.getSource() == btnRegister){
				GUI.isAccessingExcel = true;
				new ThreadRegister(this).start();
			}else if(e.getSource() == btnSearch){
				GUI.isAccessingExcel = true;
				new ThreadSesarch(this).start();
			}else if(e.getSource() == btnDelete){
				GUI.isAccessingExcel = true;
				new ThreadDelete(this).start();
			}
		
	}
}
