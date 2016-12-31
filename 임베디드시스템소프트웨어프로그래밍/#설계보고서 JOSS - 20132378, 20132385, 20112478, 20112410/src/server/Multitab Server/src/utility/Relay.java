package utility;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;


public class Relay {
	final Object lock = new Object();
	String path;
	
	public Relay(String path) {
		this.path = path;
	}
	
	public int get_relay(String index){
		ProcessBuilder pb;
		Process process;
		BufferedReader br;
		String value = "";
		
		try{
			synchronized(lock){
//				String[] cmd = {"/bin/bash","-c","echo sudo "+path+"/get_relay",index};
//				process = Runtime.getRuntime().exec(cmd);
//				pb = new ProcessBuilder("sudo -s");
//				process = pb.start();
				
				pb = new ProcessBuilder(path+"/get_relay",index);
				pb.directory(new File(path));
				process = pb.start();
				br = new BufferedReader(new InputStreamReader(process.getInputStream()));
				process.waitFor();
			}
			
//			br.readLine();
			value = br.readLine();
			
		}catch(Exception e){
			e.printStackTrace();
		}
		
		if(value == null)
			return -1;
		
		return Integer.parseInt(value);
	}

	public void set_relay(String index, String value){
		ProcessBuilder pb;
		Process process;
		
		try{
			synchronized(lock){
				pb = new ProcessBuilder(path+"/set_relay",index,value);
//				String[] cmd = {"/bin/bash","-c","echo sudo "+path+"/set_relay",index,value};
//				process = Runtime.getRuntime().exec(cmd);
				pb.directory(new File(path));
				process = pb.start();
				process.waitFor();
			}
				
		}catch(Exception e){
			e.printStackTrace();
		}
		
	}
}
