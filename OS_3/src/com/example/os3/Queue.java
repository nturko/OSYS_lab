package com.example.os3;

public class Queue {
    private int n;
    private Request[] queue;
    private int defaultPriority;

    public Queue(int defaultPriority, int n) {
        this.defaultPriority=defaultPriority;
        this.n=n;
        queue = new Request[n];
    }
    public void addRequest(Request request) {
        int i=n;
        do  {
            i--;
        }
        while ((i>0)&(queue[i]!=null));
        if ((i==0)&(queue[i]!=null)) {
            ExtendQueue(2*n);
            i=n/2-1;
        }
        queue[i]=request;
    }

    public String getQueue() {
        String res = "";
        for (int i=n-1;i>=0;i--) {
            if (queue[i]!=null) {
                res=res+queue[i].getProcessed()+"/"+queue[i].getWeight()+ " ";
            }
        }
        return res;
    }
    public boolean isEmpty() {
        if (queue[n-1]==null) {
            return true;
        }
        else
            return false;
    }
    public Request ExtractRequest() {
        Request request;
        request=queue[n-1];
        for (int i=n-2;i>=0; i--) {
            queue[i+1]=queue[i];
        }
        queue[0]=null;
        return request;
    }
    public void ExtendQueue(int n) {
        Request[] newQueue = new Request[n];
        for (int i=this.n-1; i>=0;i--) {
            newQueue[i+n-this.n]=queue[i];
        }
        queue=newQueue;
        this.n=n;
    }

}
