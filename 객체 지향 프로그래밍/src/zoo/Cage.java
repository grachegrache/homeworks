package zoo;

import java.awt.Dimension;
import java.awt.Point;

public class Cage{
    private Dimension cageDimension;
    private Point cagePoint;

    /**
     * Construct Cage
     * 
     * @param inputPoint
     */
    public Cage(Point inputPoint){
        cagePoint = inputPoint;
        cageDimension = new Dimension(300, 180);
    }

    /** @return the cageDimension */
    public Dimension getCageDimension(){
        return cageDimension;
    }

    /** @return the cagePoint */
    public Point getCagePoint(){
        return cagePoint;
    }

    public boolean isItInside(int x, int y){
        if(cagePoint.x <= x && cagePoint.y <= y)
            if(cagePoint.x + cageDimension.width >= x && cagePoint.y + cageDimension.height >= y)
                return true;
            else
                return false;
        return false;
    }

    /**
     * If it is inside, return true, or return false.
     * 
     * @param mouse
     * @return
     */
    public boolean isItInside(Point mouse){
        if(cagePoint.x <= mouse.x && cagePoint.y <= mouse.y)
            if(cagePoint.x + cageDimension.width >= mouse.x && cagePoint.y + cageDimension.height >= mouse.y)
                return true;
            else
                return false;
        return false;
    }

    /**
     * If it is inside, return true, or return false.
     * 
     * @param nextX
     * @param nextY
     * @return
     */
    public boolean isItInside(Point nextPoint, Dimension nextDimension){
        if(cagePoint.x <= nextPoint.x && cagePoint.y <= nextPoint.y)
            if(cagePoint.x + cageDimension.width >= nextPoint.x + nextDimension.width
            && cagePoint.y + cageDimension.height >= nextPoint.y + nextDimension.height)
                return true;
        return false;
    }
}
