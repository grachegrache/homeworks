package zoo;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Controller extends JPanel{
    JComboBox<String> animalComboBox;
    JButton animalDrawButton;
    JLabel animalLabel;
    JComboBox<String> backgroundBox;
    JButton cageDrawButton;
    JLabel cageLabel;
    JButton clearButton;
    ActionListener listener;
    int mode;
    Sound sound;

    Controller(){
        mode = 0;
        cageLabel = new JLabel("Cage");
        cageDrawButton = new JButton("Select");
        animalLabel = new JLabel("Animal");
        animalDrawButton = new JButton("Select");
        clearButton = new JButton("Clear");
        animalComboBox = new JComboBox<String>();
        {
            String[] animalName = {"¾ç", "µÅÁö", "Æë±Ï", "±â¸°", "ÄÚ³¢¸®", "¿ø¼þÀÌ"};
            for(String element : animalName)
                animalComboBox.addItem(element);
        }
        backgroundBox = new JComboBox<String>();
        {
            String[] background = {"³²±Ø", "À©µµ¿ì ÃÊ¿ø", "Á¤±Û", "ÁøÈë"};

            for(String element : background)
                backgroundBox.addItem(element);

            backgroundBox.addActionListener(new ActionListener(){

                @Override
                public void actionPerformed(ActionEvent arg0){
                    Monitor.monitor.image = Toolkit.getDefaultToolkit().getImage(backgroundBox.getSelectedItem() + ".jpg");
                }

            });
        }
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        {
            add(cageLabel);
            add(cageDrawButton);
            animalDrawButton.setSelected(true);
            add(animalLabel);
            add(animalDrawButton);
            add(animalComboBox);
            add(Box.createVerticalStrut(30));
            add(clearButton);
            add(Box.createVerticalStrut(30));
            add(backgroundBox);
            add(Box.createVerticalStrut(30));
        }
        listener = new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent e){
                if(e.getSource().equals(cageDrawButton)){
                    mode = 1;	// ¿ì¸® ±×¸®±â
                    animalDrawButton.setEnabled(true);
                    animalComboBox.setEnabled(false);
                    cageDrawButton.setEnabled(false);
                }else if(e.getSource().equals(animalDrawButton)){
                    mode = 2;	// µ¿¹° ±×¸®±â
                    animalDrawButton.setEnabled(false);
                    animalComboBox.setEnabled(true);
                    cageDrawButton.setEnabled(true);
                }
            }
        };
        {
            cageDrawButton.addActionListener(listener);
            animalDrawButton.addActionListener(listener);
        }
        sound = new Sound(this);
    }
}
