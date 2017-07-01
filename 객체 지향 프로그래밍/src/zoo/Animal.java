package zoo;

import java.awt.BasicStroke;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Shape;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.util.Random;

public class Animal extends Thread{
    public Dimension bound;
    public Cage fence;
    public Point location;
    boolean b;
    boolean moving;
    int page;
    Scene scene0;	// first scene.
    Scene scene1;	// second scene.
    private int dx;
    private int dy;
    private Random R;

    public Animal(Cage fence, int selection){
        this.fence = fence;
        page = 0;
        R = new Random();
        scene0 = new Scene(selection);
        scene1 = new Scene(selection + 6);
        Point a = scene0.findLeftPivot();
        Point b = scene0.findRightPivot();
        bound = new Dimension(b.x - a.x, b.y - a.y);
        convertFigureBound(selection);
        convertFigureBound(selection + 6);
        location =
                new Point(R.nextInt(fence.getCageDimension().width - bound.width) + fence.getCagePoint().x, R.nextInt(fence
                        .getCageDimension().height - bound.height)
                        + fence.getCagePoint().y);
        moving = true;
    }

    public void draw(Graphics2D graphics2d){
        Scene now = scene1;
        if(b)
            page++;
        switch(page / 10 % 2){
            case 0:
                now = scene0;
                break;
            case 1:
                now = scene1;
                break;
            default:
                break;
        }
        scene0.convertFigureLocation(location);
        scene1.convertFigureLocation(location);
        for(int i = 0; i < now.figure.size(); i++){
            graphics2d.setColor(now.color.get(i));
            graphics2d.setStroke(now.stroke.get(i));
            if(now.transparent.get(i))
                graphics2d.draw(now.figure.get(i));
            else if(whatShape(now.figure.get(i)).equals("Line"))
                graphics2d.draw(now.figure.get(i));
            else
                graphics2d.fill(now.figure.get(i));
        }
    }

    public boolean isItInside(int x, int y){
        return location.x < x && location.y < y && location.x + bound.width > x && location.y + bound.height > y;
    }

    public void move(int dx, int dy){
        if(fence.isItInside(new Point(location.x + dx, location.y + dy), bound))
            location.translate(dx, dy);
    }

    @Override
    public void run(){
        while(Manager.T.isRunning()){
            synchronized(this){
                if(!moving)
                    try{
                        this.wait();
                    }catch(InterruptedException e){
                        e.printStackTrace();
                    }
            }
            if(R.nextInt(100) > 91)
                if(R.nextBoolean()){
                    dx = R.nextInt(10) - 5;
                    dy = R.nextInt(10) - 5;
                    b = true;
                }else{
                    dx = 0;
                    dy = 0;
                    b = false;
                }
            move(dx, dy);
            try{
                sleep(30);
            }catch(InterruptedException e){
                e.printStackTrace();
            }
        }
    }

    public String whatShape(Shape figure){
        if(figure instanceof Rectangle2D.Double)
            return "Rectangle";
        else if(figure instanceof Ellipse2D.Double)
            return "Ellipse";
        else if(figure instanceof Line2D.Double)
            return "Line";
        else
            return null;
    }

    /**
     * width, height 0: pig -> 60, 30 1: penguin -> 30, 50
     * 
     * @param selection
     */
    private void convertFigureBound(int selection){
        double proportionX;
        double proportionY;
        switch(selection){
            case AnimalType.SHEEP:
                proportionX = 80.0 / bound.width;
                proportionY = 50.0 / bound.height;
                break;
            case AnimalType.PIG:
                proportionX = 90.0 / bound.width;
                proportionY = 50.0 / bound.height;
                break;
            case AnimalType.PENGUIN:
                proportionX = 30.0 / bound.width;
                proportionY = 50.0 / bound.height;
                break;
            case AnimalType.MONKY:
                proportionX = 50.0 / bound.width;
                proportionY = 50.0 / bound.height;
                break;
            default:
                proportionX = 1;
                proportionY = 1;
                break;
        }
        Point leftPivot = scene0.findLeftPivot();
        for(int i = 0; i < 2; i++){
            Scene now = scene0;
            switch(i % 2){
                case 0:
                    now = scene0;
                    break;
                case 1:
                    now = scene1;
                    break;
                default:
                    break;
            }
            for(int j = 0; j < now.stroke.size(); j++)
                now.stroke.set(j,
                        new BasicStroke(now.stroke.get(j).getLineWidth() * (float) Math.min(proportionX, proportionY)));
            for(Shape next2 : now.figure)
                switch(whatShape(next2)){
                    case "Rectangle":
                        now.rectangle2d = (Rectangle2D.Double) next2;
                        now.rectangle2d.x = leftPivot.x + (now.rectangle2d.x - leftPivot.x) * proportionX;
                        now.rectangle2d.y = leftPivot.y + (now.rectangle2d.y - leftPivot.y) * proportionY;
                        now.rectangle2d.width *= proportionX;
                        now.rectangle2d.height *= proportionY;
                        break;
                    case "Line":
                        now.line2d = (Line2D.Double) next2;
                        now.line2d.x1 = leftPivot.x + (now.line2d.x1 - leftPivot.x) * proportionX;
                        now.line2d.y1 = leftPivot.y + (now.line2d.y1 - leftPivot.y) * proportionY;
                        now.line2d.x2 = leftPivot.x + (now.line2d.x2 - leftPivot.x) * proportionX;
                        now.line2d.y2 = leftPivot.y + (now.line2d.y2 - leftPivot.y) * proportionY;
                        break;
                    case "Ellipse":
                        now.ellipse2d = (Ellipse2D.Double) next2;
                        now.ellipse2d.x = leftPivot.x + (now.ellipse2d.x - leftPivot.x) * proportionX;
                        now.ellipse2d.y = leftPivot.y + (now.ellipse2d.y - leftPivot.y) * proportionY;
                        now.ellipse2d.width *= proportionX;
                        now.ellipse2d.height *= proportionY;
                        break;
                    default:
                        break;
                }
            Point rightPivot = scene0.findRightPivot();
            bound.setSize(rightPivot.x - leftPivot.x, rightPivot.y - leftPivot.y);
        }
    }
}

interface AnimalType{
    int ELEPHANT = 4;
    int GIRAFFE = 3;
    int MONKY = 5;
    int PENGUIN = 2;
    int PIG = 1;
    int SHEEP = 0;
}
