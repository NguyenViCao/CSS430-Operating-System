/*
 * This is a template of QueueNode.java. Change this file name into QueueNode.java
 * and implement the logic.
 */

import java.util.*;

public class QueueNode {
    private Vector<Integer> tidQueue; // maintains a list of child TIDs who called wakeup( ).

    public QueueNode( ) {
	    // Implement this constructor.
        tidQueue = new Vector<>();
    }

    public synchronized int sleep( ) {
	// If tidQueue has nothing, call wait( ).
        if(tidQueue.size() == 0)
        {
            try
            {
                wait();
            }
            catch(InterruptedException e)
            {
                //catch error that occurred
                SysLib.cerr(e.toString() + "\n");
            }
            return tidQueue.remove(0);
        }
	    // Otherwise, get one child TID from tidQueue.
	    // return it.
        return -1;
    }

    public synchronized void wakeup( int tid ) {
	    // Add this child TID to tidQueue.
        tidQueue.add(tid);
	    // Notify the parent.
        notify();
    }
}
