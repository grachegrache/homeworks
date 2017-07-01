package zoo;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Point;
import java.awt.Shape;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.util.ArrayList;

public class Scene{
    public ArrayList<Color> color;
    public ArrayList<Shape> figure;
    public ArrayList<BasicStroke> stroke;
    public ArrayList<Boolean> transparent;
    Ellipse2D.Double ellipse2d;
    Line2D.Double line2d;
    Rectangle2D.Double rectangle2d;

    public Scene(int selection){
        stroke = new ArrayList<BasicStroke>();
        FileInputStream fis;
        ObjectInputStream ois;
        try{
            fis = new FileInputStream("draw.dat" + selection);
            ois = new ObjectInputStream(fis);
            figure = (ArrayList<Shape>) ois.readObject();
            color = (ArrayList<Color>) ois.readObject();
            transparent = (ArrayList<Boolean>) ois.readObject();
            for(int i = 0; i < figure.size(); i++)
                stroke.add(new BasicStroke(ois.readFloat()));
            ois.close();
            fis.close();
        }catch(Exception e){
            System.out.println("파일 Input또는 Output에 오류가 생겼습니다.");
            e.printStackTrace();
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

    void convertFigureLocation(Point location){
        Point leftPivot = findLeftPivot();
        double dx = location.x - leftPivot.x;
        double dy = location.y - leftPivot.y;
        for(Shape next : figure)
            switch(whatShape(next)){
                case "Rectangle":
                    rectangle2d = (Rectangle2D.Double) next;
                    rectangle2d.x += dx;
                    rectangle2d.y += dy;
                    break;
                case "Line":
                    line2d = (Line2D.Double) next;
                    line2d.x1 += dx;
                    line2d.x2 += dx;
                    line2d.y1 += dy;
                    line2d.y2 += dy;
                    break;
                case "Ellipse":
                    ellipse2d = (Ellipse2D.Double) next;
                    ellipse2d.x += dx;
                    ellipse2d.y += dy;
                    break;
                default:
                    break;
            }
    }

    /**
     * find and return left-upper corner Point.
     * 
     * @return
     */
    Point findLeftPivot(){
        double smallX = Double.MAX_VALUE;
        double smallY = Double.MAX_VALUE;
        for(Shape next : figure)
            switch(whatShape(next)){
                case "Rectangle":
                    rectangle2d = (Rectangle2D.Double) next;
                    if(smallX > rectangle2d.x)
                        smallX = rectangle2d.x;
                    if(smallY > rectangle2d.y)
                        smallY = rectangle2d.y;
                    break;
                case "Line":
                    line2d = (Line2D.Double) next;
                    if(smallX > Math.min(line2d.x1, line2d.x2))
                        smallX = Math.min(line2d.x1, line2d.x2);
                    if(smallY > Math.min(line2d.y1, line2d.y2))
                        smallY = Math.min(line2d.y1, line2d.y2);
                    break;
                case "Ellipse":
                    ellipse2d = (Ellipse2D.Double) next;
                    if(smallX > ellipse2d.x)
                        smallX = ellipse2d.x;
                    if(smallY > ellipse2d.y)
                        smallY = ellipse2d.y;
                    break;
                default:
                    break;
            }
        return new Point((int) smallX, (int) smallY);
    }

    /**
     * find and return right-under corner Point.
     * 
     * @return
     */
    Point findRightPivot(){
        double bigX = Double.MIN_VALUE;
        double bigY = Double.MIN_VALUE;

        for(Shape next : figure)
            switch(whatShape(next)){
                case "Rectangle":
                    rectangle2d = (Rectangle2D.Double) next;
                    if(bigX < rectangle2d.x + rectangle2d.width)
                        bigX = rectangle2d.x + rectangle2d.width;
                    if(bigY < rectangle2d.y + rectangle2d.height)
                        bigY = rectangle2d.y + rectangle2d.height;
                    break;
                case "Line":
                    line2d = (Line2D.Double) next;
                    if(bigX < Math.max(line2d.x1, line2d.x2))
                        bigX = Math.max(line2d.x1, line2d.x2);
                    if(bigY < Math.max(line2d.y1, line2d.y2))
                        bigY = Math.max(line2d.y1, line2d.y2);
                    break;
                case "Ellipse":
                    ellipse2d = (Ellipse2D.Double) next;
                    if(bigX < ellipse2d.x + ellipse2d.width)
                        bigX = ellipse2d.x + ellipse2d.width;
                    if(bigY < ellipse2d.y + ellipse2d.height)
                        bigY = ellipse2d.y + ellipse2d.height;
                    break;
                default:
                    break;
            }
        return new Point((int) bigX, (int) bigY);
    }
}
