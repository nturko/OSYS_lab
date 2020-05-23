package com.example.os3;

public class Request {
    private int weight;
    private int priority;
    private int processed;
    private int inTime;
    private int outTime;

    public Request(int weight, int priority, int inTime) {
        this.weight = weight;
        this.priority = priority;
        this.inTime = inTime;
        processed=0;
    }

    public int getWeight() {
        return weight;
    }

    public int getPriority() {
        return priority;
    }
    public void incProcessed() {
        processed++;
    }
    public int getProcessed() {
        return processed;
    }
    public void setInTime(int inTime) {
        this.inTime = inTime;
    }
    public void setOutTime(int outTime) {
        this.outTime = outTime;
    }
    public int getInTime() {
        return inTime;
    }
    public int getOutTime() {
        return outTime;
    }
}
