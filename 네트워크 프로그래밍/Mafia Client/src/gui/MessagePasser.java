package gui;

import java.util.StringTokenizer;

import javax.swing.JOptionPane;

/**
 * 메시지의 파싱이나 언파싱을 담당하는 클래스입니다.
 * @author Administrator
 *
 */

public class MessagePasser {
	
	StringTokenizer tokenizer; //메시지를 파싱하는데에 쓸 tokenizer함수입니다.
	String headString; //메시지의 헤드입니다.
	String senderString; //메시지의 송신자입니다.
	String bodyString; //메시지의 본체입니다.
	
	Process process;
	
	public MessagePasser(Process process) 
	{
		this.process = process;
	}
	
	public void passMessage(String msg)
	{
		if (!msg.isEmpty())
		{
			tokenizer = new StringTokenizer(msg,"\n");
			headString = tokenizer.nextToken();
			bodyString = tokenizer.nextToken();
			senderString = bodyString;
			if (senderString.equals("서버"))
			{
				bodyString = tokenizer.nextToken();
				bodyString = bodyString.concat(tokenizer.nextToken());
			}
			else if (tokenizer.hasMoreTokens())
			{
				bodyString = tokenizer.nextToken();
			}
			checkMessageHead();
		}
		
	}
	
	private void checkMessageHead()
	{
		if(headString.equals("Started"))
		{
			process.mainFrame.chatTextArea.append(senderString+" started game\n");
		}
		else if (headString.equals("Accept"))
		{
			process.acceptedPlayer(senderString);
		}
		else if (headString.equals("Chatted"))
		{
			process.updateChatMessage(senderString, bodyString);
		}
		else if (headString.equals("Choosen"))
		{
			if (senderString.equals("Fail"))
			{
				process.updateChatNotice("투표가 실패했습니다.");
				return;
			}
			else
				process.choosenPlayer(senderString);
		}
		else if (headString.equals("Error"))
		{
			JOptionPane.showMessageDialog(null, senderString,"서버에서 에러",JOptionPane.ERROR_MESSAGE);
		}
		else if (headString.equals("Exited"))
		{
			String exitedString = String.format("플레이어 <%s> 게임 종료", senderString);
			process.updateChatNotice(exitedString);
			process.ExitedPlayer(senderString);
		}
		else if (headString.equals("Invited"))
		{
			if(senderString.equals(process.userName)){
				process.updateChatNotice("비밀 서버에 초대되었습니다.");
				process.invitedSecretChat();
			}
		}
		else if (headString.equals("State"))
		{
			process.changeState(senderString);
		}
		else if (headString.equals("GEnd"))
		{
			process.GameEnd(senderString, bodyString);
		}
	}
}
