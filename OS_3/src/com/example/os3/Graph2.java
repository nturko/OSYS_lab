package com.example.os3;

import java.awt.Color;
import java.awt.Graphics;


public class Graph2 extends Graph {
    public Graph2(Form form,short[] value, String x, String y) {
        super(form,value,x,y);
        setSize(510,300);
    }
    public void paint(Graphics g) {
        g.setColor(Color.BLACK);
        DrawAxises(g);
        g.drawOval(50, 280-value[1]*2,1,1);
        g.drawString(String.valueOf(value[form.getIMax()-1]),40+(form.getIMax()-1)*10,280-value[form.getIMax()-1]*2);
        for (int i=form.getIMin()+1; i<form.getIMax(); i++) {
            g.setColor(Color.BLACK);
            g.drawOval(40+i*10, 280-value[i]*2, 1, 1);
            g.setColor(Color.RED);
            g.drawLine(40+(i-1)*10, 280-value[i-1]*2, 40+(i)*10, 280-value[i]*2);
        }

    }
    public void DrawAxises(Graphics g) {
        g.drawString(y,10,40);
        g.drawString(x,410,293);
        g.drawLine(40, 40, 40, 280);
        g.drawLine(40, 280, 480, 280);
        for (int i=0; i<11;i++) {
            g.drawString(String.valueOf(i*10),15,280-i*20);
            g.drawLine(38,280-i*20,42,280-i*20);
        }
        for (int i=0; i<9; i++) {
            g.drawString(String.valueOf(i*4),40+i*40,293);
            g.drawLine(40+i*40, 282, 40+i*40, 278);
        }
        for (int i=9; i<12;i++) {
            g.drawString(String.valueOf(i*4),40+i*40, 278);
            g.drawLine(40+i*40, 282, 40+i*40, 278);
        }
    }
}

