package gui;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import main.Constant;

public class GUI{
	public static boolean	isAccessingExcel;
	private JTabbedPane		panel;
	
	public GUI(){
		isAccessingExcel = false;
		
		JFrame frm = new JFrame("Money Book");
		frm.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frm.setBounds(200, 150, 640, 480);
		frm.setResizable(false);
		setPanel();
		frm.add(panel);
		frm.setVisible(true);
	}
	
	private void setPanel(){
		panel = new JTabbedPane();
		JPanel outcome = new MoneyPanel(Constant.out);
		JPanel income = new MoneyPanel(Constant.in);
		JPanel report = new ReportPanel();
		
		panel.add("지출내역", outcome);
		panel.add("수입내역", income);
		panel.add("자료통계", report);
		
	}
}
