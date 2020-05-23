package com.example.os3;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import javax.swing.border.LineBorder;
public class Form extends JFrame {
    private JLabel iminLabel = new JLabel("Мин. t поступления: ");
    private JLabel imaxLabel = new JLabel("Макс. t поступления: ");
    private JTextField imin = new JTextField(5);
    private JTextField imax = new JTextField(5);
    private JLabel wminLabel = new JLabel("Мин. вес: ");
    private JLabel wmaxLabel = new JLabel("Макс. вес: ");
    private JTextField wmin = new JTextField(5);
    private JTextField wmax = new JTextField(5);
    private JLabel prminLabel = new JLabel("Мин. приоритет: ");
    private JLabel prmaxLabel = new JLabel("Макс. приоритет: ");
    private JTextField prmin = new JTextField(5);
    private JTextField prmax = new JTextField(5);
    private JPanel iPanel = new JPanel();
    private JPanel wPanel = new JPanel();
    private JPanel prPanel = new JPanel();
    private JPanel fieldsPanel = new JPanel();
    private JPanel buttonsPanel = new JPanel();
    private JPanel textPanel = new JPanel();
    private JTextArea area = new JTextArea(20,50);
    private JScrollPane scroll = new JScrollPane(area);
    private JButton tact = new JButton("Выполнить такт");
    private JButton graph1 = new JButton("График Тож(I)");
    private JButton graph2 = new JButton("График idle(I)");
    private JButton graph3 = new JButton("График Тож(Pr)");
    private JButton graphs = new JButton("Построение графиков");
    private JButton init = new JButton("Задать значения");
    private JTextField quant = new JTextField(5);
    private JLabel quantLabel = new JLabel("Квант времени: ");
    private JPanel quantPanel = new JPanel();
    private JButton values = new JButton("Задать новые значения");
    private JLabel nLabel = new JLabel("Кол-во тактов: ");
    private JTextField nField = new JTextField(5);
    //------------------------------------------------------------------//
    private int iminValue;
    private int imaxValue;
    private int wminValue;
    private int wmaxValue;
    private int prminValue;
    private int prmaxValue;
    private int time = 0;
    private int waitingTime;
    private int quantValue;
    private int waitingTime_all=0;
    private int requestsNum = 0;
    private QueueBlock queue;
    private Resource resource;



    public Form() {
        super("Lab3");
        setSize(600,500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        Interface();
        setVisible(true);
        init.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                Init();
                setFieldsEditable(false);
                init.setVisible(false);
                tact.setVisible(true);
                graphs.setVisible(true);
                values.setVisible(true);
            }
        });

        tact.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                ExecuteOneTact(queue, resource);
                Visualisation(queue, resource);
            }
        });
        values.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                values.setVisible(false);
                setFieldsEditable(true);
                graph1.setVisible(false);
                graph2.setVisible(false);
                graph3.setVisible(false);
                graphs.setVisible(false);
                tact.setVisible(false);
                init.setVisible(true);
            }
        });
        graphs.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                tact.setVisible(false);
                nField.setEditable(true);
                graphs.setVisible(false);
                graph1.setVisible(true);
                graph2.setVisible(true);
                graph3.setVisible(true);
                values.setVisible(true);
            }
        });
        graph1.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                int min = iminValue;
                int max = imaxValue;
                short[] array = new short[imaxValue+iminValue+1];
                for (int i=1;i<=max;i++) {
                    imaxValue=i;
                    iminValue=i;
                    QueueBlock testBlock = new QueueBlock(prmaxValue-prminValue,prminValue);
                    resource.setQueue(testBlock);
                    resource.setRequest(null);
                    for (short j=0;j<Integer.parseInt(nField.getText());j++) {
                        ExecuteOneTact(testBlock, resource);
                    }
                    array[iminValue]=(short)(waitingTime_all/resource.getProcessedNumber());
                    waitingTime_all=0;

                }
                iminValue=min;
                imaxValue=max;
                resource.setQueue(queue);
                Init();
                new Graph(Form.this, array, "Тпоступления","Tожидания");
            }


        });
        graph2.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                int min = iminValue;
                int max = imaxValue;
                short[] array = new short[imaxValue+iminValue+1];
                for (int i=1;i<max;i++) {
                    imaxValue=i;
                    iminValue=i;
                    QueueBlock testBlock = new QueueBlock(prmaxValue-prminValue,prminValue);
                    resource.setQueue(testBlock);
                    resource.setRequest(null);
                    for (short j=0;j<Integer.parseInt(nField.getText());j++) {
                        ExecuteOneTact(testBlock, resource);
                    }
                    array[iminValue]=(short)(((float)resource.getIdleTime()/time)*100);
                    time=0;
                    resource.setIdleTime(0);

                }
                iminValue=min;
                imaxValue=max;
                resource.setQueue(queue);
                Init();
                new Graph2(Form.this, array, "Тпоступления","% простоя");
            }

        });
        graph3.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                Init();
                QueueBlock testBlock = new QueueBlock(prmaxValue-prminValue,prminValue);
                resource.setQueue(testBlock);
                resource.setRequest(null);

                for (short j=0;j<Integer.parseInt(nField.getText());j++) {
                    ExecuteOneTact(testBlock, resource );
                }
                int[] array = resource.getPriorityArray();
                short[] numbers = resource.getProcessedNumberWithPriorities();
                Request r;
                while ((r = testBlock.ExtractMaxPriorityRequest())!=null) {
                    numbers[r.getPriority()]++;
                    array[r.getPriority()]+=getTime()-r.getInTime()-r.getProcessed();
                }
                for (int i=1;i<array.length; i++) {

                    if (numbers[i]!=0) {
                        array[i]/=numbers[i];
                    }
                }
                resource.setQueue(queue);
                new Graph3(Form.this, array, "Приоритет","Тожидания");

            }
        });
    }
    public void Interface() {
        getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
        getContentPane().add(fieldsPanel);
        getContentPane().add(buttonsPanel);
        getContentPane().add(textPanel);
        fieldsPanel.setBorder(new LineBorder(Color.BLACK));
        fieldsPanel.add(iPanel);
        fieldsPanel.add(wPanel);
        fieldsPanel.add(prPanel);
        fieldsPanel.add(quantPanel);
        iPanel.setLayout(new BoxLayout(iPanel, BoxLayout.Y_AXIS));
        wPanel.setLayout(new BoxLayout(wPanel, BoxLayout.Y_AXIS));
        prPanel.setLayout(new BoxLayout(prPanel, BoxLayout.Y_AXIS));
        quantPanel.setLayout(new BoxLayout(quantPanel, BoxLayout.Y_AXIS));
        iPanel.add(iminLabel);
        iPanel.add(imin);
        iPanel.add(imaxLabel);
        iPanel.add(imax);
        wPanel.add(wminLabel);
        wPanel.add(wmin);
        wPanel.add(wmaxLabel);
        wPanel.add(wmax);
        prPanel.add(prminLabel);
        prPanel.add(prmin);
        prPanel.add(prmaxLabel);
        prPanel.add(prmax);
        quantPanel.add(quantLabel);
        quantPanel.add(quant);
        quantPanel.add(nLabel);
        quantPanel.add(nField);
        nField.setText("30000");
        nField.setEditable(false);
        buttonsPanel.setBorder(new LineBorder(Color.BLUE));
        buttonsPanel.add(init);
        tact.setVisible(false);
        graphs.setVisible(false);
        values.setVisible(false);
        graph1.setVisible(false);
        graph2.setVisible(false);
        graph3.setVisible(false);
        buttonsPanel.add(tact);
        buttonsPanel.add(graphs);
        tact.setToolTipText("Выполнить один такт");
        graph1.setToolTipText("Построить график зависимости среднего времени ожидания от интенсивности поступления заявок");
        graph2.setToolTipText("Построить график зависимости процента простоя ресурса от интенсивности поступления заявок");
        graph3.setToolTipText("Построить график зависимости среднего времени ожидания от приоритета");
        buttonsPanel.add(graph1);
        buttonsPanel.add(graph2);
        buttonsPanel.add(graph3);
        buttonsPanel.add(values);
        textPanel.add(scroll);
        area.setEditable(false);
        setVisible(true);

    }
    private void Init() {
        time = 0;
        requestsNum = 0;
        iminValue = Integer.parseInt(imin.getText());
        imaxValue = Integer.parseInt(imax.getText());
        wminValue = Integer.parseInt(wmin.getText());
        wmaxValue = Integer.parseInt(wmax.getText());
        prminValue = Integer.parseInt(prmin.getText());
        prmaxValue = Integer.parseInt(prmax.getText());
        quantValue = Integer.parseInt(quant.getText());
        queue = new QueueBlock(prmaxValue-prminValue, prminValue);
        waitingTime = iminValue+(int)(Math.round(Math.random()*(imaxValue-iminValue)));
        resource = new Resource(this, queue, quantValue);
        resource.setRequest(null);
    }
    public void ExecuteOneTact(QueueBlock queue, Resource resource) {
        resource.processOneTact();
        time++;
        waitingTime--;
        if (waitingTime == 0) {
            queue.addRequestWithFilter(new Request(wminValue+(int)(Math.round(Math.random()*(wmaxValue-wminValue))),prminValue+(int)(Math.round(Math.random()*(prmaxValue-prminValue))),time));
            requestsNum+=1;
            waitingTime = iminValue+(int)(Math.round(Math.random()*(imaxValue-iminValue)));
        }
    }
    public void Visualisation(QueueBlock queue, Resource resource) {
        area.setText(queue.getBlock());
        if (resource.getCurrentRequest()!=null) {
            area.append("Обработка текущей заявки: "+resource.getCurrentRequest().getProcessed()+"/"+resource.getCurrentRequest().getWeight()+ "(Приоритет: "+ resource.getCurrentRequest().getPriority()+") \n");
            area.append("Квант: "+String.valueOf(resource.getQuant())+ "\n");
            area.append("Время пребывания текущей заявки на ресурсе "+String.valueOf(resource.getProcessingTime())+"\n");
        }
        else {
            area.append("Ресурс простаивает. Общее время простоя: "+String.valueOf(resource.getIdleTime())+ "\n");
            area.append("Процент простоя: "+ ((float)resource.getIdleTime()/time)*100 +"\n");
        }
        area.append("Общее время работы системы: "+getTime()+" \n");
        area.append("Текущее кол-во заявок в системе: "+getRequestsNumber()+"\n");
        area.append("Заявок обработано: "+resource.getProcessedNumber()+"\n");

    }
    public void addWaitingTime(int time) {
        waitingTime_all+=time;
    }
    public int getRequestsNumber() {
        return requestsNum;
    }
    public void setRequestsNumber(int num) {
        requestsNum = num;
    }
    public int getTime() {
        return time;
    }
    public int getIMin() {
        return iminValue;
    }
    public int getIMax() {
        return imaxValue;
    }
    public void setFieldsEditable(boolean value) {
        imin.setEditable(value);
        imax.setEditable(value);
        wmin.setEditable(value);
        wmax.setEditable(value);
        prmin.setEditable(value);
        prmax.setEditable(value);
        quant.setEditable(value);
    }
    public int getPrMin() {
        return prminValue;
    }
    public int getPrMax() {
        return prmaxValue;
    }
    public static void main(String[] args) {

        new Form();
    }

}

