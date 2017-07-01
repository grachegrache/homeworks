package zoo;

import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.Timer;

public class Manager{
    static Timer T;				// Panel has animals and cages.
    private Monitor P;		// Timer repaints and have animals move.
    private Viewer V;		// Viewer creates Jframe.

    public Manager(){
        P = new Monitor();
        V = new Viewer();
        V.controller.clearButton.addActionListener(new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent arg0){
                P.clear();
            }
        });
        getMouseListener listener = new getMouseListener();
        P.addMouseListener(listener);
        P.addMouseMotionListener(listener);
        V.frm.add(P);
        T = new Timer(30, new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent arg0){
                P.repaint();
            }
        });
        // P.T = T;
        T.start();
    }

    private class getMouseListener extends MouseAdapter{

        Point beforeLocation;
        Point beforeMouse;
        Animal target;

        @Override
        public void mouseClicked(MouseEvent e){
            boolean enable = true;

            if(e.getButton() == MouseEvent.BUTTON3)
                if(P.mouse != null)
                    if(V.controller.mode == 1){
                        if(P.cages.isEmpty())
                            P.cages.add(new Cage(P.mouse));
                        else{
                            for(Cage next1 : P.cages)
                                if(next1.isItInside(P.mouse) || next1.isItInside(P.mouse.x + 300, P.mouse.y)
                                        || next1.isItInside(P.mouse.x, P.mouse.y + 180)
                                        || next1.isItInside(P.mouse.x + 300, P.mouse.y + 180))
                                    enable = false;
                            if(enable)
                                P.cages.add(new Cage(P.mouse));
                        }
                    }else
                        for(Cage next2 : P.cages)
                            if(next2.isItInside(P.mouse)){
                                P.animals.add(new Animal(next2, V.controller.animalComboBox.getSelectedIndex()));
                                P.animals.get(P.animals.size() - 1).start();
                                return;
                            }
        }

        @Override
        public void mouseDragged(MouseEvent e){
            if(target == null)
                return;
            target.location.x = beforeLocation.x + P.mouse.x - beforeMouse.x;
            target.location.y = beforeLocation.y + P.mouse.y - beforeMouse.y;
        }

        @Override
        public void mousePressed(MouseEvent e){
            if(e.getButton() == MouseEvent.BUTTON1)
                for(Animal animal : P.animals)
                    if(animal.isItInside(P.mouse.x, P.mouse.y)){
                        animal.moving = false;
                        beforeMouse = new Point(P.mouse.x, P.mouse.y);
                        beforeLocation = new Point(animal.location.x, animal.location.y);
                        target = animal;
                    }
        }

        @Override
        public void mouseReleased(MouseEvent e){
            if(target == null)
                return;
            if(e.getButton() == MouseEvent.BUTTON1){
                target.moving = true;
                synchronized(target){
                    target.notify();
                }
                for(Cage cage : P.cages)
                    if(cage.isItInside(P.mouse))
                        target.fence = cage;
                target = null;
            }
        }
    }
}
