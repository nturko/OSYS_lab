package com.example.os3;

import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JFrame;


public class Graph3 extends JFrame {
    protected int value[];
    protected Form form;
    protected String x,y;
    public Graph3(Form form, int[] value, String x, String y) {
        super("График зависимости ср. времени ожидания от приоритета заявок");
        this.form=form;
        this.x=x;
        this.y=y;
        this.value=value;
        setSize(700, 610);
        setVisible(true);
    }
    public void paint(Graphics g) {
        g.setColor(Color.BLACK);
        DrawAxises(g);
        g.drawOval(40+form.getPrMin()*20, 580-value[form.getPrMin()],1,1);
        g.drawString(String.valueOf(value[form.getPrMin()]),38+20, 578-value[form.getPrMin()]);
        for (int i=form.getPrMin()+1; i<=form.getPrMax(); i++) {
            g.setColor(Color.BLACK);
            g.drawOval(40+i*20, 580-value[i], 1, 1);
            g.setColor(Color.RED);
            g.drawLine(40+(i-1)*20, 580-value[i-1], 40+(i)*20, 580-value[i]);
            g.setColor(Color.BLACK);
            g.drawString(String.valueOf(value[i]),38+i*20, 578-value[i]);
        }
    }
    public void DrawAxises(Graphics g) {
        g.drawString(y,10,40);
        g.drawString(x,610,593);
        g.drawLine(40, 40, 40, 580);
        g.drawLine(40, 580, 680, 580);
        for (int i=0; i<27;i++) {
            g.drawString(String.valueOf(i*20),15,580-i*20);
            g.drawLine(38,580-i*20,42,580-i*20);
        }
        for (int i=0; i<13; i++) {
            g.drawString(String.valueOf(2*i),40+i*40,593);
            g.drawLine(40+i*40, 582, 40+i*40, 578);
        }

        for (int i=13; i<16;i++) {
            g.drawString(String.valueOf(2*i),40+i*40, 578);
            g.drawLine(40+i*40, 582, 40+i*40, 578);
        }

    }
}
