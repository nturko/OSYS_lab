package com.example.os3;

public class QueueBlock {
    private Queue[] queueBlock;
    private int number;
    private int minPriority;

    public QueueBlock(int n, int minPriority) {
        number = n+1;
        this.minPriority = minPriority;
        queueBlock = new Queue[number];
        BlockInitialization();
    }

    private void BlockInitialization() {
        for (int i=0;i<number;i++) {
            queueBlock[i] = new Queue(i+minPriority,1);

        }
    }
    public void addRequestWithFilter(Request request) {
        queueBlock[request.getPriority()-minPriority].addRequest(request);
    }
    public void addRequestDirectly(Request request, int queueNumber) {
        queueBlock[queueNumber].addRequest(request);
    }
    public String getBlock() {
        String res = "";
        for (int i=0;i<number;i++) {
            res=res+("Приоритет: "+(i+minPriority)+" | ");
            res=res+queueBlock[i].getQueue()+"\n";
        }
        return res;
    }
    public Request ExtractMaxPriorityRequest() {
        int i = -1;
        do {
            i++;
        }
        while ((i<number-1)&(queueBlock[i].isEmpty()));
        if (queueBlock[number-1]!=null) {
            if (i!=number-1)
                return queueBlock[i].ExtractRequest();
            else
                return queueBlock[number-1].ExtractRequest();
        }
        else
            return null;

    }
    public int getMinPriority() {
        return minPriority;
    }
    public int getQueueNumber() {
        return number;
    }


}
