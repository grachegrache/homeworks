package gui;

import gui.thread.ThreadReport;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import main.Constant;

public class ReportPanel extends JPanel implements ActionListener{
	public JButton		btnPerMonth;
	public JButton		btnPerWeek;
	public JButton		btnPerYear;
	public JCheckBox	chkIncome;
	public JCheckBox	chkOutcome;
	public JTextArea	text;
	
	public ReportPanel(){
		chkOutcome = new JCheckBox("지출통계");
		chkIncome = new JCheckBox("수입통계");
		btnPerWeek = new JButton("주단위");
		btnPerMonth = new JButton("월단위");
		btnPerYear = new JButton("년단위");
		text = new JTextArea();
		
		btnPerWeek.addActionListener(this);
		btnPerMonth.addActionListener(this);
		btnPerYear.addActionListener(this);
		
		JScrollPane scrollPane = new JScrollPane(text);
		
		JPanel p1 = new JPanel();
		JPanel p2 = new JPanel();
		p1.setPreferredSize(new Dimension(200, 50));
		p2.setPreferredSize(new Dimension(300, 50));
		text.setPreferredSize(new Dimension(600, 350));
		// setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		
		p1.add(chkOutcome);
		p1.add(chkIncome);
		p2.add(btnPerWeek);
		p2.add(btnPerMonth);
		p2.add(btnPerYear);
		
		add(p1);
		add(p2);
		add(scrollPane);
		
	}
	
	@Override
	public void actionPerformed(ActionEvent e){
		class ThreadForAction extends Thread{
			ActionEvent	e;
			ReportPanel	panel;
			
			public ThreadForAction(ReportPanel panel, ActionEvent e){
				this.panel = panel;
				this.e = e;
			}
			
			@Override
			public void run(){
				boolean isPrevExist = false;
				Thread th = null;
				
				if(chkIncome.isSelected()){
					isPrevExist = true;
					text.setText("수입내역\n");
					if(e.getSource() == btnPerWeek)
						th = new ThreadReport(panel, Constant.week, Constant.in);
					else if(e.getSource() == btnPerMonth)
						th = new ThreadReport(panel, Constant.month, Constant.in);
					else if(e.getSource() == btnPerYear)
						th = new ThreadReport(panel, Constant.year, Constant.in);
					
					if(th != null){
						th.start();
						while(th.isAlive())
							;
					}
				}
				
				th = null;
				
				if(chkOutcome.isSelected()){
					if(isPrevExist)
						text.append("\n지출내역\n");
					else
						text.setText("\n지출내역\n");
					if(e.getSource() == btnPerWeek)
						th = new ThreadReport(panel, Constant.week, Constant.out);
					else if(e.getSource() == btnPerMonth)
						th = new ThreadReport(panel, Constant.month, Constant.out);
					else if(e.getSource() == btnPerYear)
						th = new ThreadReport(panel, Constant.year, Constant.out);
					
					if(th != null){
						th.start();
						while(th.isAlive())
							;
						GUI.isAccessingExcel = false;
					}
				}
			}
		}
		
		if(GUI.isAccessingExcel == false){
			GUI.isAccessingExcel = true;
			new ThreadForAction(this, e).start();
		}
	}
	
}
