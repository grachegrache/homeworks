package zoo;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import javazoom.jl.player.Player;

public class Sound{
    File file;
    FileInputStream fis;
    BufferedInputStream bis;
    Player p;
    PlayButtonListener pbl;
    boolean loop;
    JButton play;

    public Sound(JPanel panel){
        try{
            file = new File("Apink - 난 니가 필요해.mp3");
        }catch(Exception e){
            e.printStackTrace();
        }

        JLabel music = new JLabel("Music");

        play = new JButton("Play");
        play.addActionListener(pbl = new PlayButtonListener());

        JButton stop = new JButton("Stop");
        stop.addActionListener(new StopButtonListener());

        panel.add(music);
        panel.add(play);
        panel.add(stop);
    }

    class PlayButtonListener extends Thread implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent arg0){
            try{
                if(!isAlive())
                    start();
            }catch(Exception e){
                e.printStackTrace();
            }
        }

        @Override
        public void interrupt(){
            loop = false;
            p.close();
            super.interrupt();
        }

        @Override
        public void run(){
            loop = true;
            try{
                do{
                    fis = new FileInputStream(file);
                    bis = new BufferedInputStream(fis);
                    p = new Player(bis);
                    p.play();
                }while(loop);
            }catch(Exception e){
                e.printStackTrace();
            }
        }
    }

    class StopButtonListener implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent e){
            try{
                pbl.interrupt();
                play.removeActionListener(pbl);
                play.addActionListener(pbl = new PlayButtonListener());
            }catch(Exception e1){
                e1.printStackTrace();
            }
        }
    }
}
