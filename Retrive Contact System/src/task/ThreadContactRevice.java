package task;

import main.Main;
import gui.GUIContact;

public class ThreadContactRevice extends Thread{
	GUIContact instance;
	Contact contact;
	
	public ThreadContactRevice(GUIContact instance, Contact contact){
		this.instance = instance;
		this.contact = contact;
	}
	
	@Override
	public void run(){
		String name = instance.txtName.getText();
		String tel = instance.txtPhone.getText();
		String group = instance.txtGroup.getText();
		
		try{
			if(Main.groupList.contains(group)){
				contact.name = name;
				contact.tel = tel;
				contact.group = group;
				Main.contactList.set(Main.contactList.indexOf(contact), contact);
				Main.saveContact();
			}
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
	}
}
