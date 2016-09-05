package task;

import main.Main;
import gui.GUIContact;

public class ThreadContactRegister extends Thread{
	GUIContact instance;
	
	public ThreadContactRegister(GUIContact instance){
		this.instance = instance;
	}
	
	@Override
	public void run(){
		String name = instance.txtName.getText();
		String tel = instance.txtPhone.getText();
		String group = instance.txtGroup.getText();
		
		try{
			if(Main.groupList.contains(group)){
				Main.contactList.add(new Contact(name, tel, group));
				Main.saveContact();
			}
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
	}
}
