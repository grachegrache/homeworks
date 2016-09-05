package gui;

import javax.swing.JOptionPane;

public class Main {
	
	public static void main(String[] args) {
		String userName;
		String IPAddress = "127.0.0.1";
		int PortNumber = 10000;
		int secretPortNumber = 10001;
		Log log = new Log("log.txt");
		
		while(true)
		{
			userName = JOptionPane.showInputDialog("유저 이름을 입력하세요. 유저 이름은 Fail을 제외한 10자 이하의 이름이어야합니다.");
			
			if (userName.equals("Fail") || userName.equals("서버"))
			{
				JOptionPane.showMessageDialog(null, "해당 값은 이름으로 할 수 없습니다.");
				continue;
			}
			else if (userName.length() > 10)
			{
				JOptionPane.showMessageDialog(null, "이름은 길이 10을 넘을 수 없습니다.");
				continue;
			}
			break;
		}
		
		MainFrame frame = new MainFrame(userName, IPAddress, PortNumber, log);
		frame.setVisible(true);
		SecretChatFrame scrtFrame = new SecretChatFrame(userName, IPAddress, secretPortNumber, log);
		scrtFrame.process.mainFrame = frame;
		frame.process.secretFrame = scrtFrame;
		
	}
}
