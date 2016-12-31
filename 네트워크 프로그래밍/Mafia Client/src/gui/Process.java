package gui;

import java.io.IOException;
import java.net.Socket;
import java.util.Random;

import javax.swing.JOptionPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.table.DefaultTableModel;

/**
 * 클라이언트 프로그램의 기본적인 시스템들은 여기서 수행되고, 저장됩니다.
 * @author Administrator
 *
 */

public class Process {
	Level currentLevel; //현재 단계를 나타내는 변수입니다.
	boolean MAFIA; //이 플레이어가 마피아인지 아닌지를 나타내는 값입니다.
	boolean life; //이 플레이어가 살아있는지 아닌지를 나타내는 값입니다.
	boolean invited;
	boolean voted;
	boolean isMain;
	
	String userName; //해당 유저의 이름입니다.
	String missionUserName; //해당 유저의 미션인 이름입니다.
	Socket socket; //게임 서버에 접속하는 소켓입니다.
	
	MainFrame mainFrame; //프레임 레퍼런스입니다. 이 값을 통해서 프레임의 값을 변경시킵니다.
	SecretChatFrame secretFrame; //비밀 채팅의 프레임입니다. 위와 하는 역할은 동일합니다. 하지만 전체 채팅 메시지가 전송되면 이 프레임은 자동으로 꺼집니다.
	MessagePasser messagePasser; //수신 메시지의 파싱을 담당하는 클래스입니다.
	//각각 송신, 수신을 담당하는 클래스입니다.
	Sender sender;
	RecieverThread recieverThread;
	Random random = new Random();
	
	//로그 관련을 담당하는 인스턴스입니다.
	Log log;
	
	public Process(MainFrame frame, Log log)
	{
		currentLevel = Level.READY;
		MAFIA = false;
		life = true;
		invited = false;
		voted = false;
		if(frame instanceof SecretChatFrame)
			isMain=false;
		else
			isMain=true;
		
		mainFrame = frame;
		this.userName = new String(frame.userName);
		this.log = log;
		
		try {
			socket = new Socket(frame.IPAddress, frame.portNumber);
			recieverThread = new RecieverThread(socket,this);
			sender = new Sender(socket);
			
			recieverThread.start();
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "서버 연결에 실패했습니다.", "서버 연결 오류", JOptionPane.ERROR_MESSAGE);
		}
		
		if(isMain)
			accessServer();
		
	}
	
	public void setLog(Log log)
	{
		this.log = log;
	}
	
	public void choiceMissionName()
	{
		int choosenIndex = 0;
		String choosenName = "";
		
		choosenIndex = random.nextInt(10) % mainFrame.playerTable.getRowCount();
		choosenName = (String) mainFrame.tableModel.getValueAt(choosenIndex, 0);
		
		missionUserName = new String(choosenName);
		mainFrame.missionNameLabel.setText("미션:" + missionUserName);
//		mainFrame.update(mainFrame.getGraphics());
	}
	
	//이 이후부터는 메시지를 보내는 함수들입니다.
	
	/**
	 * 게임 서버에 접속 메시지를 보냅니다. 접속 메시지에는 플레이어의 이름이 존재합니다.
	 */
	public void accessServer()
	{
		String sendMessage = new String("Access\n");
		sendMessage = sendMessage.concat(new String(userName));
		sender.sendMessage(sendMessage);
		log.addLogAboutSendMessage(sendMessage);
	}
	
	/**
	 * 게임 서버에 시작 메시지를 보냅니다.
	 */
	public void StartGame()
	{
		String sendMessage = new String("Start");
		sender.sendMessage(sendMessage);
		log.addLogAboutSendMessage(sendMessage);
	}
	
	/**
	 * 게임 서버에 채팅 메시지를 보냅니다.
	 * @param message 채팅 메시지입니다.
	 */
	public void chatAllServer(String message)
	{
		if (currentLevel != Level.DAY && currentLevel != Level.NIGHT)
		{
			JOptionPane.showMessageDialog(null, "채팅 단계가 아닙니다.","단계 오류",JOptionPane.ERROR_MESSAGE);
			return;
		}
		if (message.isEmpty())
		{
			return;
		}
		//메시지를 보내는데에는 문제가 없습니다. 이제 비밀 채팅 서버를 닫을 때입니다.
		if (secretFrame.isVisible())
		{
			ExitSecretServer();
		}
		if (message.contains(missionUserName))
		{
			mainFrame.chatTextArea.append("미션 클리어! 비밀 채팅창을 열 수 있습니다!\n");
			mainFrame.btnSecretChat.setEnabled(true);
		}
		
		String sendMessage = new String("Chat\n");
		sendMessage = sendMessage.concat(message);
		
		sender.sendMessage(sendMessage);
		log.addLogAboutSendMessage(sendMessage);
	}
	
	/**
	 * 비밀 서버에 채팅을 보냅니다.
	 * @param message 채팅 메시지입니다.
	 */
	public void chatSecretServer(String message)
	{
		if (currentLevel != Level.DAY)
		{
			JOptionPane.showMessageDialog(null, "채팅 단계가 아닙니다.","단계 오류",JOptionPane.ERROR_MESSAGE);
			return;
		}
		if (message.isEmpty())
		{
			return;
		}
		String sendMessage = new String("Chat\n");
		sendMessage = sendMessage.concat(message);
		
		secretFrame.process.sender.sendMessage(sendMessage);
		log.addLogAboutSendMessage(sendMessage);
	}
	
	/**
	 * 비밀 서버에 나갈 때 작동하는 메시지입니다. 만약 SecreatChatFrame에 직접 접근, 
	 * 종료메시지를 보내고 종료하는 방법이 가능하면 이 함수는 무시해도 됩니다.
	 */
	public void ExitSecretServer()
	{
		invited = false;
		String sendMessage = new String("Exit\n");

		secretFrame.setVisible(false); //이제 시크릿 프레임을 제거합니다.
		log.addLogAboutSendMessage(sendMessage);
	}
	
	/**
	 * 전체 서버 = 게임 서버에 나갈 때 작동하는 메시지입니다.
	 */
	public void ExitAllServer()
	{
		String sendMessage = new String("Exit\n");
		
		if (life)
			sender.sendMessage(sendMessage);
		
		if (secretFrame != null)
		{
			ExitSecretServer();
			secretFrame.dispose();
			secretFrame = null;
		}
		mainFrame.dispose();
		
		recieverThread.interrupt();
		
		log.addLogAboutSendMessage(sendMessage);
	}
	
	/**
	 * 투표메시지를 전송하는 함수입니다.
	 * @param selectedName 선택한 사람입니다.
	 */
	public void VotePlayer(String selectedName)
	{
		if(voted)
			return;
		else{
			voted = true;
			
			if (currentLevel != Level.VOTE)
			{
				JOptionPane.showMessageDialog(null, "투표 단계가 아닙니다.", "단계 오류", JOptionPane.ERROR_MESSAGE);
				return;
			}
			if (selectedName.isEmpty())
			{
				return;
			}
			String sendMessage = new String("Choice\n");
			sendMessage = sendMessage.concat(selectedName);
			sender.sendMessage(sendMessage);
			log.addLogAboutSendMessage(sendMessage);
		}
	}
	
	/**
	 * 비밀 서버를 열고 사람들에게 초대메시지를 보내는 함수입니다.
	 */
	public void inviteSecretChat() {
		int[] selectedPlayer = mainFrame.playerTable.getSelectedRows();
		String sendMessage = null;
		StringBuilder sb = new StringBuilder();
		if (selectedPlayer.length == 0)
		{
			JOptionPane.showMessageDialog(null, "초대할 플레이어를 선택하세요", "비밀 채팅 오류", JOptionPane.ERROR_MESSAGE);
			return;
		}
		if(selectedPlayer.length == 1)
			if(mainFrame.playerTable.getValueAt(selectedPlayer[0], 0).equals(new String(userName))){
				JOptionPane.showMessageDialog(null, "초대할 플레이어를 선택하세요", "비밀 채팅 오류", JOptionPane.ERROR_MESSAGE);
				return;
			}
		
//		sendMessage = new String("Invite\n");
//		sb.append(sendMessage.concat(new String(userName))+"\n");
		
		for (int index = 0; index < selectedPlayer.length; index++)
		{
			sendMessage = new String("Invite\n");
			String recieverName = (String) mainFrame.playerTable.getValueAt(selectedPlayer[index], 0);
			sb.append(sendMessage.concat(recieverName));
			if(index != selectedPlayer.length-1)
				sb.append("\n");
			//String[] invitedPlayer = new String[1];
			//invitedPlayer[0] = recieverName;
			//secretFrame.tableModel.addRow(invitedPlayer);
		}
		String invites = sb.toString();
		secretFrame.process.sender.sendMessage(invites);
		log.addLogAboutSendMessage(invites);
		
		if (!secretFrame.isVisible())
		{
			secretFrame.chatTextArea.setText("비밀 채팅방이 열렸습니다.\n");
			secretFrame.setVisible(true);
			log.addLogAboutSendMessage(sendMessage);
		}
	}
	
	
	
	
	
	
	
	//여기서부터는 MessagePasser가 호출하는 함수입니다.
	
	
	
	
	
	
	
	
	
	
	/**
	 * Accept메시지를 받으면 작동하는 함수입니다.
	 * @param senderName
	 */
	public void acceptedPlayer(String senderName)
	{
		String playerName;
		JTable playerTable;
		if(isMain)
			playerTable = mainFrame.playerTable;
		else
			playerTable = secretFrame.playerTable;
		
		if (playerTable == null)
		{
			return;
		}
		
		if (playerTable.getRowCount() > 0)
		{
			for (int index = 0; index < playerTable.getRowCount(); index++)
			{
				playerName = (String) playerTable.getValueAt(index, 0);
				if (playerName.equals(senderName))
					return;
			}
		}
		String[] addedRow = new String[1];
		addedRow[0] = senderName;
		
		DefaultTableModel model = (DefaultTableModel) playerTable.getModel();
		model.addRow(addedRow);
		
		
		
		choiceMissionName();
	}
	
	/**
	 * JTextArea에 받은 공지 메시지를 출력하는 함수입니다.
	 * @param notice 송신한 공지입니다.
	 */
	public synchronized void updateChatNotice(String notice)
	{
		mainFrame.chatTextArea.append(notice + "\n");
	}
	
	/**
	 * JTextArea에 받은 메시지를 출력하는 함수입니다.
	 * @param senderName 송신자의 이름입니다.
	 * @param message 수신된 메시지의 내용입니다.
	 */
	public synchronized void updateChatMessage(String senderName, String message)
	{
		String inputMessage = "";
		JTextArea chatTextArea;
		if(isMain)
			chatTextArea = mainFrame.chatTextArea;
		else
			chatTextArea = secretFrame.chatTextArea;
		
		if (senderName.equals("서버"))
		{
			if (message.contains("마피아"))
			{
				MAFIA = true;
				mainFrame.roleLabel.setText("역할:마피아");
			}
			else
			{
				mainFrame.roleLabel.setText("역할:시민");
			}
		}
		inputMessage = "<" + senderName + "> : " + message; 
		chatTextArea.append(inputMessage + "\n");
	}
	
	/**
	 * Exited메시지를 받으면 작동하는 함수입니다.
	 * @param senderName 메시지에 있는 송신자 메시지입니다.
	 */
	public void ExitedPlayer(String senderName)
	{
		int playerNumber = 0;
		String playerName = "";
		JTable playerTable;
		if(isMain)
			playerTable = mainFrame.playerTable;
		else
			playerTable = secretFrame.playerTable;
		
		for (int index = 0; index < playerTable.getRowCount(); index++)
		{
			playerName = (String) playerTable.getValueAt(index, 0);
			if (playerName.equals(senderName))
			{
				playerNumber = index;
				break;
			}
		}
		
		DefaultTableModel model = (DefaultTableModel) playerTable.getModel();
		model.removeRow(playerNumber);
		
		if(isMain)
			mainFrame.chatTextArea.append(senderName + " 님이 게임에서 나갔습니다.\n");
		else
			secretFrame.chatTextArea.append(senderName + " 님이 게임에서 나갔습니다.\n");
	}
	/**
	 * 서버에서 보낸 메시지에 따라 상태를 바꿉니다.
	 * @param bodyString 
	 */
	public void changeState(String bodyString) {
		String levelString = "";
		if (bodyString.contains("Ready"))
		{
			currentLevel = Level.DAY;
			levelString = "준비";
		}
		else if (bodyString.contains("Day"))
		{
			currentLevel = Level.DAY;
			levelString = "회의 - 낮";
			if (!MAFIA)
			{
				mainFrame.btnSendMessage.setEnabled(true);
				mainFrame.btnSendMessage.setEnabled(true);
			}
		}
		else if (bodyString.contains("Night"))
		{
			currentLevel = Level.NIGHT;
			levelString = "회의 - 밤";
			if (!MAFIA)
			{
				mainFrame.btnSendMessage.setEnabled(false);
				mainFrame.btnSendMessage.setEnabled(false);
			}
			
		}
		else if (bodyString.contains("Vote"))
		{
			currentLevel = Level.VOTE;
			levelString = "투표";
		}
		mainFrame.LevelLabel.setText("단계:" + levelString);
	}
	/**
	 * 게임에서 처형된 사람의 이름을 채팅창에 올려주는 함수입니다.
	 * @param choosenPlayer
	 */
	public void choosenPlayer(String choosenPlayer)
	{
		mainFrame.chatTextArea.append(choosenPlayer + "님이 처형되었습니다.\n");
		if (choosenPlayer.equals(userName))
		{
			life = false;
			mainFrame.chatTextArea.append("플레이어가 죽었습니다.\n");
		}
		ExitedPlayer(choosenPlayer);
	}
 
	/**
	 * 비밀 채팅방으로 초대받을 때 작동하는 메소드입니다.
	 */
	public void invitedSecretChat() {
		invited = true;
		mainFrame.chatTextArea.append("비밀 채팅방이 열렸습니다.\n 비밀 채팅방은 전체 채팅을 하면 자동으로 꺼집니다.");
		secretFrame.process.accessServer();
		secretFrame.chatTextArea.setText("비밀 채팅방이 열렸습니다.");
		secretFrame.setVisible(true);
	}
	/**
	 * 게임 결과를 채팅창으로 알려주는 메소드입니다.
	 * @param winnedPlayer
	 * @param mapiaNames
	 */
	public void GameEnd(String winnedPlayer, String mapiaNames)
	{
		String resultMessage = "";
		if (winnedPlayer.isEmpty())
		{
			resultMessage = "승자를 정하지 못한 채로 게임이 종료되었습니다.";
		}
		else if (winnedPlayer.startsWith("Mafia"))
		{
			resultMessage = "마피아의 승리입니다.";
		}
		else if (winnedPlayer.startsWith("Citizen"))
		{
			resultMessage = "시민의 승리입니다.";
		}
		
		mainFrame.chatTextArea.append(resultMessage+"\n");
		mainFrame.chatTextArea.append("마피아는 " + mapiaNames + "였습니다.\n");
		log.addGamePlayMessage("승리자:" + winnedPlayer + "마피아였던 플레이어:" + mapiaNames);
	}
}
