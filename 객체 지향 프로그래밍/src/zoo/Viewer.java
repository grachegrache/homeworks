package zoo;

import java.awt.BorderLayout;

import javax.swing.JFrame;

public class Viewer{
    Controller controller;
    JFrame frm;

    public Viewer(){
        frm = new JFrame("zoo");
        frm.setBounds(200, 100, 1000, 600);
        frm.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        controller = new Controller();
        frm.add(controller, BorderLayout.WEST);
        frm.setVisible(true);
    }

}
