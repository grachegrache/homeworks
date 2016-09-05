package gui;

import java.awt.Font;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.border.BevelBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;


public class MainFrame extends JFrame {

	String userName;
	String IPAddress;
	int portNumber;
	
	Process process;

	private JPanel contentPane;
	TextField txtRoomChatWrite;
	JButton btnExit;
	JButton btnSendMessage;
	JTextArea chatTextArea;
	JLabel LevelLabel;
	JLabel userNameLabel;
	JLabel roleLabel;
	JLabel missionNameLabel;

	JButton btnVote;
	JButton btnSecretChat;
	JTable playerTable;
	DefaultTableModel tableModel;
	private String[] columnsName = {"참가 플레이어"};
	private JButton stratButton;
	



	

	@SuppressWarnings("rawtypes")
	public MainFrame(String userName, String IPAddress, int portNumber, Log log) {
		
		this.userName = userName;
		this.IPAddress = IPAddress;
		this.portNumber = portNumber;
		
		this.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				process.ExitAllServer();
				super.windowClosed(e);
			}
		});
		

		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setBounds(150, 150, 570, 484);
        setTitle("마피아 게임");
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);

		contentPane.setLayout(null);

		
		JPanel leftPanel = new JPanel();
		leftPanel.setBorder(new BevelBorder(BevelBorder.LOWERED, null, null, null, null));
		leftPanel.setBounds(12, 43, 324, 383);
		contentPane.add(leftPanel);
		leftPanel.setLayout(null);

		txtRoomChatWrite = new TextField();
		txtRoomChatWrite.setBounds(10, 290, 304, 33);
		leftPanel.add(txtRoomChatWrite);
		
		btnExit = new JButton("나가기");
		btnExit.setBounds(234, 329, 78, 44);
		btnExit.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				process.ExitAllServer();
				System.exit(0);
			}
		});
		leftPanel.add(btnExit);
		
		btnSendMessage = new JButton("보내기");
		btnSendMessage.setBounds(106, 329, 116, 44);
		btnSendMessage.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				process.chatAllServer(txtRoomChatWrite.getText());
			}
		});
		leftPanel.add(btnSendMessage);
		
		chatTextArea = new JTextArea();
		chatTextArea.setBounds(12, 10, 300, 274);
		leftPanel.add(chatTextArea);
		
		stratButton = new JButton("게임 시작");
		stratButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				process.StartGame();
			}
		});
		stratButton.setBounds(10, 329, 84, 44);
		leftPanel.add(stratButton);
		
		
		JPanel rightPanel = new JPanel();
		rightPanel.setBorder(new BevelBorder(BevelBorder.LOWERED, null, null, null, null));
		rightPanel.setBounds(348, 0, 166, 426);
		contentPane.add(rightPanel);
		rightPanel.setLayout(null);

		

		TextField textField_1 = new TextField();
		textField_1.setBounds(46, 9, 78, 21);
		textField_1.setText("플레이어");
		textField_1.setFont(new Font("Dialog", Font.BOLD, 17));
		textField_1.setEnabled(false);
		textField_1.setEditable(false);
		rightPanel.add(textField_1);
						
		btnVote = new JButton("투표");
		btnVote.setBounds(53, 300, 71, 44);
		btnVote.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if (playerTable.getSelectedRowCount() == 1)
				{
					int selectedIndex = playerTable.getSelectedRow();
					String selectedName = (String) playerTable.getValueAt(selectedIndex, 0);
					process.VotePlayer(selectedName);
				}
				else
					JOptionPane.showMessageDialog(null, "투표는 1명의 플레이어에게 해야합니다.");
			}
		});
		rightPanel.add(btnVote);
		
		btnSecretChat = new JButton("비밀 채팅");
		btnSecretChat.setBounds(35, 370, 108, 44);
		btnSecretChat.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if (process.secretFrame.isVisible())
				{
					return;
				}
				else if(process.secretFrame.process.invited == false)
				{
					process.secretFrame.process.accessServer();
					process.inviteSecretChat();
					
				}else
				{
					process.secretFrame.setVisible(true);
				}
			}
		});
		btnSecretChat.setEnabled(false);
		rightPanel.add(btnSecretChat);
		
		
		playerTable = new JTable();
		tableModel = new DefaultTableModel(columnsName,0);
		playerTable.setModel(tableModel);
		JScrollPane scrollPane = new JScrollPane(playerTable);
		scrollPane.setBounds(12, 89, 142, 199);
		rightPanel.add(scrollPane);

		userNameLabel = new JLabel("이름:" + userName);
		userNameLabel.setBounds(12, 39, 142, 15);
		rightPanel.add(userNameLabel);
		
		roleLabel = new JLabel("역할: 미정");
		roleLabel.setBounds(12,57,142,15);
		rightPanel.add(roleLabel);
		
		missionNameLabel = new JLabel("미션:");
		missionNameLabel.setBounds(12,73,142,15);
		rightPanel.add(missionNameLabel);

		TextField textField = new TextField();
		textField.setEnabled(false);
		textField.setEditable(false);
		textField.setFont(new Font("Dialog", Font.BOLD, 17));
		textField.setText("채팅 창");
		textField.setBounds(136, 10, 65, 23);
		contentPane.add(textField);

		LevelLabel = new JLabel("단계:처음");
		LevelLabel.setBounds(207, 18, 129, 15);
		contentPane.add(LevelLabel);

		process = new Process(this,log);
		
	}
}