package com.example.os3;

public class Resource {
    private Request currentRequest;
    private int processingTime;
    private int quant;
    private int idleTime = 0;
    private boolean occupied = false;
    private QueueBlock queue;
    private int counter = 0;
    private Form form;
    private int[] priorities;
    private short[] number;
    public Resource(Form form, QueueBlock queue, int quant) {
        this.form = form;
        this.queue = queue;
        this.quant = quant;
        priorities = new int[queue.getMinPriority()+queue.getQueueNumber()+1];
        number = new short[queue.getMinPriority()+queue.getQueueNumber()+1];
        for (int i=0;i<priorities.length;i++) {
            priorities[i]=0;
            number[i] = 0;
        }
    }
    public void setRequest(Request request) {
        currentRequest = request;
        processingTime = 0;
        if (request!=null) {
            occupied = true;
        }
        else
            occupied = false;
    }
    public void processOneTact() {
        if (occupied) {
            currentRequest.incProcessed();
            processingTime++;
            if (currentRequest.getProcessed()>=currentRequest.getWeight()) {
                currentRequest.setOutTime(form.getTime());
                form.addWaitingTime(currentRequest.getOutTime()-currentRequest.getInTime()-currentRequest.getWeight());
                int waitingTime=(form.getTime()-currentRequest.getInTime()-currentRequest.getWeight());
                priorities[currentRequest.getPriority()]+=waitingTime;
                number[currentRequest.getPriority()]++;
                setRequest(queue.ExtractMaxPriorityRequest());
                form.setRequestsNumber(form.getRequestsNumber()-1);
                counter++;

            }
            if (processingTime>=quant) {
                queue.addRequestDirectly(currentRequest, currentRequest.getPriority()-queue.getMinPriority());
                setRequest(queue.ExtractMaxPriorityRequest());
            }

        }
        else {
            idleTime++;
            setRequest(queue.ExtractMaxPriorityRequest());
        }

    }
    public Request getCurrentRequest() {
        return currentRequest;
    }
    public int getQuant() {
        return quant;
    }
    public int getProcessingTime() {
        return processingTime;
    }
    public int getIdleTime() {
        return idleTime;
    }
    public void setIdleTime(int time) {
        idleTime = time;
    }
    public int getProcessedNumber() {
        return counter;
    }
    public void setQueue(QueueBlock queue) {
        this.queue=queue;
    }
    public int[] getPriorityArray() {
        return priorities;
    }
    public short[] getProcessedNumberWithPriorities() {
        return number;
    }
}
