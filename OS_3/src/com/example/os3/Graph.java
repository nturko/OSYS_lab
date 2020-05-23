package com.example.os3;

import java.awt.Color;
import java.awt.Graphics;

import javax.swing.*;

public class Graph extends JFrame{
    protected short[] value;
    protected Form form;
    protected String x,y;

    public Graph(Form form, short[] value, String x, String y) {
        super("График");
        this.form=form;
        setSize(510, 600);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setResizable(false);
        this.value = value;
        this.x=x;
        this.y=y;
        setVisible(true);
    }

    public void paint(Graphics g) {
        g.setColor(Color.BLACK);
        DrawAxises(g);
        g.drawOval(50, 580-value[1]*2,1,1);
        if ((580-value[1]*2)<30) {
            g.drawString(String.valueOf(value[1]),70,60);
        }
        g.drawString(String.valueOf(value[1]),60,580-value[1]*2);
        for (int i=form.getIMin()+1; i<=form.getIMax(); i++) {
            g.setColor(Color.BLACK);
            if ((value[i]!=32767)) {
                g.drawOval(40+i*10, 580-value[i]*2, 1, 1);
            }
            g.setColor(Color.RED);
            if (value[i-1]!=32767) {
                g.drawLine(40+(i-1)*10, 580-value[i-1]*2, 40+(i)*10, 580-value[i]*2);
            }
        }

    }
    public void DrawAxises(Graphics g) {
        g.drawString(y,10,40);
        g.drawString(x,410,593);
        g.drawLine(40, 40, 40, 580);
        g.drawLine(40, 580, 480, 580);
        for (int i=0; i<27;i++) {
            g.drawString(String.valueOf(i*10),15,580-i*20);
            g.drawLine(38,580-i*20,42,580-i*20);
        }
        for (int i=0; i<9; i++) {
            g.drawString(String.valueOf(i*4),40+i*40,593);
            g.drawLine(40+i*40, 582, 40+i*40, 578);
        }
        for (int i=9; i<12;i++) {
            g.drawString(String.valueOf(i*4),40+i*40, 578);
            g.drawLine(40+i*40, 582, 40+i*40, 578);
        }
    }
}
