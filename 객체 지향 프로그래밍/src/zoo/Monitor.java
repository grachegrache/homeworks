package zoo;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import javax.swing.JPanel;

public class Monitor extends JPanel{
    public static Monitor monitor;
    public ArrayList<Animal> animals;
    public List<Cage> cages;
    public Point mouse;
    Image image;

    public Monitor(){
        super();
        super.setBackground(Color.white);
        cages = Collections.synchronizedList(new ArrayList<Cage>());
        animals = new ArrayList<Animal>();
        image = Toolkit.getDefaultToolkit().getImage("남극.jpg");

        monitor = this;
    }

    public void clear(){
        animals.clear();
        cages.clear();
    }

    @Override
    public void paintComponent(Graphics g){
        super.paintComponent(g);
        mouse = super.getMousePosition();
        g.drawImage(image, 0, 0, getWidth(), getHeight(), this);

        for(Cage c : cages)
            g.drawRect(c.getCagePoint().x, c.getCagePoint().y, c.getCageDimension().width, c.getCageDimension().height);

        for(Animal ma : animals)
            ma.draw((Graphics2D) g);

    }
}
