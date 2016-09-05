package task;

import javax.swing.table.DefaultTableModel;

import main.Main;
import gui.GUIRetrive;

public class ThreadRetrive extends Thread{
	GUIRetrive instance;
	
	public ThreadRetrive(GUIRetrive instance){
		this.instance = instance;
	}
	
	@Override
	public void run(){
		//list clear
		instance.retrivedList.clear();
		
		String keyword = instance.txtInput.getText();
		
		// 검색
		for(Contact c : Main.contactList)
			switch (instance.type){
				case 0:
					if(c.retriveName(keyword))
						instance.retrivedList.add(c);
					break;
				case 1:
					if(c.retrivePhoneNumber(keyword))
						instance.retrivedList.add(c);
					break;
				case 2:
					if(c.retriveGroup(keyword))
						instance.retrivedList.add(c);
					break;
			}
		
		DefaultTableModel model = (DefaultTableModel) instance.table.getModel();
		
		//JTable clear
		while(model.getRowCount() > 0)
			model.removeRow(0);
		
		// JTable 반영
		for(Contact c : instance.retrivedList){
			String[] row = {c.name, c.tel, c.group};
			model.addRow(row);
		}
	}
}
