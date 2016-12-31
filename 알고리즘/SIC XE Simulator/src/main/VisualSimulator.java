package main;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;

import java.awt.FileDialog;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.DefaultListModel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JList;
import javax.swing.JButton;

import org.eclipse.wb.swing.FocusTraversalOnArray;

import java.awt.Component;
import java.io.File;

public class VisualSimulator extends JFrame implements IVisualSimulator, ActionListener{
	public JTextField txtFileName;
	public JTextField txtProgName;
	public JTextField txtStartAddr;
	public JTextField txtLength;
	public JTextField txtDecA;
	public JTextField txtHexA;
	public JTextField txtDecX;
	public JTextField txtHexX;
	public JTextField txtDecL;
	public JTextField txtHexL;
	public JTextField txtDecPC;
	public JTextField txtHexPC;
	public JTextField txtSW;
	public JTextField txtDecB;
	public JTextField txtDecS;
	public JTextField txtDecT;
	public JTextField txtHexB;
	public JTextField txtHexS;
	public JTextField txtHexT;
	public JTextField txtF;
	public JTextField txt1stInst;
	public JTextField txtTarget;
	public JList listInst;
	public JTextField txtDev;
	public JTextArea txtLog;
	private JButton btnStep;
	private JButton btnAll;
	private JButton btnExit;
	private JButton btnOpen;
	
	
	public VisualSimulator(){
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		setTitle("SIC/XE \uC2DC\uBBAC\uB808\uC774\uD130");
		getContentPane().setLayout(null);
		setBounds(300, 100, 504, 712);
		
		JLabel lblFilename = new JLabel("FileName");
		lblFilename.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblFilename.setBounds(12, 10, 57, 15);
		getContentPane().add(lblFilename);
		
		txtFileName = new JTextField();
		txtFileName.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtFileName.setBounds(81, 7, 93, 21);
		getContentPane().add(txtFileName);
		txtFileName.setColumns(10);
		
		JLabel lblHheaderRecord = new JLabel("H(Header Record)");
		lblHheaderRecord.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblHheaderRecord.setBounds(12, 51, 102, 15);
		getContentPane().add(lblHheaderRecord);
		
		JLabel lblProgramName = new JLabel("Program Name");
		lblProgramName.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblProgramName.setBounds(22, 76, 92, 15);
		getContentPane().add(lblProgramName);
		
		JLabel lblStartAddress = new JLabel("Start Address");
		lblStartAddress.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblStartAddress.setBounds(22, 104, 82, 15);
		getContentPane().add(lblStartAddress);
		
		JLabel lblLength = new JLabel("Length");
		lblLength.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblLength.setBounds(22, 134, 57, 15);
		getContentPane().add(lblLength);
		
		txtProgName = new JTextField();
		txtProgName.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtProgName.setBounds(113, 73, 82, 21);
		getContentPane().add(txtProgName);
		txtProgName.setColumns(10);
		
		txtStartAddr = new JTextField();
		txtStartAddr.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtStartAddr.setBounds(113, 101, 82, 21);
		getContentPane().add(txtStartAddr);
		txtStartAddr.setColumns(10);
		
		txtLength = new JTextField();
		txtLength.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtLength.setBounds(113, 132, 82, 21);
		getContentPane().add(txtLength);
		txtLength.setColumns(10);
		
		JLabel lblRegister = new JLabel("Register");
		lblRegister.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblRegister.setBounds(12, 179, 57, 15);
		getContentPane().add(lblRegister);
		
		JLabel lblDec = new JLabel("Dec");
		lblDec.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblDec.setBounds(81, 206, 57, 15);
		getContentPane().add(lblDec);
		
		JLabel lblHex = new JLabel("Hex");
		lblHex.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblHex.setBounds(170, 206, 57, 15);
		getContentPane().add(lblHex);
		
		JLabel lblA = new JLabel("A(#0)");
		lblA.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblA.setBounds(12, 234, 57, 15);
		getContentPane().add(lblA);
		
		JLabel lblX = new JLabel("X(#1)");
		lblX.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblX.setBounds(12, 263, 57, 15);
		getContentPane().add(lblX);
		
		JLabel lblL = new JLabel("L(#2)");
		lblL.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblL.setBounds(12, 291, 57, 15);
		getContentPane().add(lblL);
		
		JLabel lblPc = new JLabel("PC(#8)");
		lblPc.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblPc.setBounds(12, 320, 57, 15);
		getContentPane().add(lblPc);
		
		JLabel lblSw = new JLabel("SW(#9)");
		lblSw.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblSw.setBounds(12, 350, 57, 15);
		getContentPane().add(lblSw);
		
		JLabel lblB = new JLabel("B(#3)");
		lblB.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblB.setBounds(12, 379, 57, 15);
		getContentPane().add(lblB);
		
		JLabel lblS = new JLabel("S(#4)");
		lblS.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblS.setBounds(12, 408, 57, 15);
		getContentPane().add(lblS);
		
		JLabel lblT = new JLabel("T(#5)");
		lblT.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblT.setBounds(12, 435, 37, 15);
		getContentPane().add(lblT);
		
		JLabel lblF = new JLabel("F(#6)");
		lblF.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblF.setBounds(12, 466, 57, 15);
		getContentPane().add(lblF);
		
		JLabel lblLog = new JLabel("Log");
		lblLog.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblLog.setBounds(12, 495, 57, 15);
		getContentPane().add(lblLog);
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(12, 520, 464, 145);
		getContentPane().add(scrollPane);
		
		txtLog = new JTextArea();
		txtLog.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 13));
		scrollPane.setViewportView(txtLog);
		
		txtDecA = new JTextField();
		txtDecA.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecA.setBounds(81, 231, 82, 21);
		getContentPane().add(txtDecA);
		txtDecA.setColumns(10);
		
		txtHexA = new JTextField();
		txtHexA.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexA.setBounds(170, 231, 82, 21);
		getContentPane().add(txtHexA);
		txtHexA.setColumns(10);
		
		txtDecX = new JTextField();
		txtDecX.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecX.setBounds(81, 260, 82, 21);
		getContentPane().add(txtDecX);
		txtDecX.setColumns(10);
		
		txtHexX = new JTextField();
		txtHexX.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexX.setBounds(170, 260, 82, 21);
		getContentPane().add(txtHexX);
		txtHexX.setColumns(10);
		
		txtDecL = new JTextField();
		txtDecL.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecL.setBounds(81, 288, 82, 21);
		getContentPane().add(txtDecL);
		txtDecL.setColumns(10);
		
		txtHexL = new JTextField();
		txtHexL.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexL.setBounds(170, 288, 82, 21);
		getContentPane().add(txtHexL);
		txtHexL.setColumns(10);
		
		txtDecPC = new JTextField();
		txtDecPC.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecPC.setBounds(81, 316, 82, 21);
		getContentPane().add(txtDecPC);
		txtDecPC.setColumns(10);
		
		txtHexPC = new JTextField();
		txtHexPC.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexPC.setBounds(169, 316, 82, 21);
		getContentPane().add(txtHexPC);
		txtHexPC.setColumns(10);
		
		txtSW = new JTextField();
		txtSW.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtSW.setColumns(10);
		txtSW.setBounds(81, 347, 171, 21);
		getContentPane().add(txtSW);
		
		txtDecB = new JTextField();
		txtDecB.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecB.setColumns(10);
		txtDecB.setBounds(81, 376, 82, 21);
		getContentPane().add(txtDecB);
		
		txtDecS = new JTextField();
		txtDecS.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecS.setColumns(10);
		txtDecS.setBounds(81, 404, 82, 21);
		getContentPane().add(txtDecS);
		
		txtDecT = new JTextField();
		txtDecT.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDecT.setColumns(10);
		txtDecT.setBounds(81, 432, 82, 21);
		getContentPane().add(txtDecT);
		
		txtHexB = new JTextField();
		txtHexB.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexB.setColumns(10);
		txtHexB.setBounds(170, 376, 82, 21);
		getContentPane().add(txtHexB);
		
		txtHexS = new JTextField();
		txtHexS.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexS.setColumns(10);
		txtHexS.setBounds(170, 404, 82, 21);
		getContentPane().add(txtHexS);
		
		txtHexT = new JTextField();
		txtHexT.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtHexT.setColumns(10);
		txtHexT.setBounds(169, 432, 82, 21);
		getContentPane().add(txtHexT);
		
		txtF = new JTextField();
		txtF.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtF.setColumns(10);
		txtF.setBounds(82, 463, 170, 21);
		getContentPane().add(txtF);
		
		JLabel lblEendRecord = new JLabel("E(End Record)");
		lblEendRecord.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblEendRecord.setBounds(274, 51, 102, 15);
		getContentPane().add(lblEendRecord);
		
		JLabel lblAddrthInst = new JLabel("Addr 1th inst");
		lblAddrthInst.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblAddrthInst.setBounds(274, 76, 75, 15);
		getContentPane().add(lblAddrthInst);
		
		txt1stInst = new JTextField();
		txt1stInst.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txt1stInst.setBounds(351, 73, 93, 21);
		getContentPane().add(txt1stInst);
		txt1stInst.setColumns(10);
		
		JLabel lblTargetAddr = new JLabel("Target Addr");
		lblTargetAddr.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblTargetAddr.setBounds(274, 206, 82, 15);
		getContentPane().add(lblTargetAddr);
		
		txtTarget = new JTextField();
		txtTarget.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtTarget.setBounds(383, 203, 93, 21);
		getContentPane().add(txtTarget);
		txtTarget.setColumns(10);
		
		JLabel lblInstructions = new JLabel("Instructions");
		lblInstructions.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		lblInstructions.setBounds(274, 234, 82, 15);
		getContentPane().add(lblInstructions);
		
		JScrollPane scrollPane_1 = new JScrollPane();
		scrollPane_1.setBounds(274, 263, 102, 218);
		getContentPane().add(scrollPane_1);
		
		listInst = new JList(new DefaultListModel());
		listInst.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		scrollPane_1.setViewportView(listInst);
		
		JLabel label = new JLabel("\uC0AC\uC6A9\uC911\uC778 \uC7A5\uCE58");
		label.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		label.setBounds(383, 234, 82, 15);
		getContentPane().add(label);
		
		txtDev = new JTextField();
		txtDev.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		txtDev.setBounds(383, 260, 93, 21);
		getContentPane().add(txtDev);
		txtDev.setColumns(10);
		
		btnStep = new JButton("\uC2E4\uD589(1step)");
		btnStep.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		btnStep.setBounds(383, 291, 97, 23);
		getContentPane().add(btnStep);
		
		btnAll = new JButton("\uC2E4\uD589(all)");
		btnAll.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		btnAll.setBounds(383, 320, 97, 23);
		getContentPane().add(btnAll);
		
		btnExit = new JButton("\uC885\uB8CC");
		btnExit.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		btnExit.setBounds(383, 350, 97, 23);
		getContentPane().add(btnExit);
		
		btnOpen = new JButton("Open");
		btnOpen.setFont(new Font("³ª´®°íµñ", Font.PLAIN, 12));
		btnOpen.setBounds(186, 6, 75, 23);
		getContentPane().add(btnOpen);
		setFocusTraversalPolicy(new FocusTraversalOnArray(new Component[]{txtFileName, btnOpen, btnStep, btnAll,
				btnExit}));
		
		btnOpen.addActionListener(this);
		btnStep.addActionListener(this);
		btnAll.addActionListener(this);
		btnExit.addActionListener(this);
		setVisible(true);
	}
	
	@Override
	public void allStep(){
		Main.simulator.allStep();
		Main.rm.affectVisualSimulator();
	}
	
	@Override
	public void initialize(){
		String filename = txtFileName.getText();
		if(filename == null || filename.length() == 0)
			return;
		
		File file = new File(filename);
		if(file.exists() == false)
			return;
		
		Main.simulator.initialize(file);
	}
	
	@Override
	public void oneStep(){
		Main.simulator.oneStep();
		Main.rm.affectVisualSimulator();
	}
	
	@Override
	public void actionPerformed(ActionEvent e){
		if(e.getSource() == btnOpen){
			FileDialog fd = new FileDialog(this, "ÆÄÀÏ ¿­±â", FileDialog.LOAD);
			fd.setVisible(true);
			txtFileName.setText(fd.getFile());
			initialize();
			
		}else if(e.getSource() == btnStep){
			oneStep();
			
		}else if(e.getSource() == btnAll){
			allStep();
			
		}else if(e.getSource() == btnExit){
			dispose();
		}
	}
}
