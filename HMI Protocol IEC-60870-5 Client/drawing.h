#ifndef DONUTR_H
#define DONUTR_H

#include <QPainter>
#include <QWidget>

class drawing : public QWidget
{
    Q_OBJECT

public:
    drawing(QWidget *parent);
    void setMiniDonut(int value, int valColor, int w, int h){
        slide = value;
        warna = valColor;
        lebar = w;
        tinggi = h;
        set = 1;
        update();
    }
    //Overview
    void set_OverviewBTR(bool stat,char val){
        valueBTR = val;
        valueBTRStat = stat;
        set = 2;
        update();
    }
    void set_OverviewBTRindi(bool val){
        valueBTRindi = val;
        set = 2;
        update();
    }
    void set_OverviewSWT(bool val){
        valueSWT = val;
        set = 2;
        update();
    }
    void set_OverviewBDI(bool stat,char val){
        valueBDI = val;
        valueBDIStat = stat;
        set = 2;
        update();
    }
    void set_OverviewBDIindi(bool val){
        valueBDIindi = val;
        set = 2;
        update();
    }
    void set_OverviewPlts(bool stat,char val){
        valuePlts = val;
        valuePltsStat = stat;
        set = 2;
        update();
    }
    void set_OverviewPltmh(bool stat,char val){
        valuePltmh = val;
        valuePltmhStat = stat;
        set = 2;
        update();
    }
    void set_OverviewLoad(bool stat,char val){
        valueLoad = val;
        valueLoadStat = stat;
        set = 2;
        update();
    }

    //PLTS
    void set_PltsInv1(bool stat, char val){
        valuePltsInv1stat = stat;
        valuePltsInv1 = val;
        set = 3;
        update();
    }
    void set_PltsInv2(bool stat, char val){
        valuePltsInv2stat = stat;
        valuePltsInv2 = val;
        set = 3;
        update();
    }
    void set_PltsInv3(bool stat, char val){
        valuePltsInv3stat = stat;
        valuePltsInv3 = val;
        set = 3;
        update();
    }
    void set_PltsOut(bool stat, char val){
        valuePltsOutstat = stat;
        valuePltsOut = val;
        set = 3;
        update();
    }

    //PLTMH
    void set_Pltmh(bool stat, char val){
        valuePltmhOutstat = stat;
        valuePltmhOut = val;
        set = 4;
        update();
    }

    //Load
    void set_Loadr(bool stat,char val){
        valueLoadRstat = stat;
        valueLoadR = val;
        set = 5;
        update();
    }
    void set_Loads(bool stat,char val){
        valueLoadTstat = stat;
        valueLoadT = val;
        set = 5;
        update();
    }
    void set_Loadt(bool stat,char val){
        valueLoadTstat = stat;
        valueLoadT = val;
        set = 5;
        update();
    }

    //Inverter
    void set_InvR(bool stat, char val){
        valueInvRstat = stat;
        valueInvR = val;
        set = 6;
        update();
    }
    void set_InvS(bool stat, char val){
        valueInvSstat = stat;
        valueInvS = val;
        set = 6;
        update();
    }
    void set_InvT(bool stat, char val){
        valueInvTstat = stat;
        valueInvT = val;
        set = 6;
        update();
    }
    void set_InvRindi(bool val){
        valueInvRdir = val;
        set = 6;
        update();
    }
    void set_InvSindi(bool val){
        valueInvSdir = val;
        set = 6;
        update();
    }
    void set_InvTindi(bool val){
        valueInvTdir = val;
        set = 6;
        update();
    }
    void set_InvRSw(bool val){
        valueInvRSw = val;
        set = 6;
        update();
    }
    void set_InvSSw(bool val){
        valueInvSSw = val;
        set = 6;
        update();
    }
    void set_InvTSw(bool val){
        valueInvTSw = val;
        set = 6;
        update();
    }
    void set_Btr(bool stat, char val){
        valueBtrOut = val;
        valueBtrOutstat = stat;
        set = 7;
        update();
    }
    void set_Btrindi(bool val){
        valueBtrOutindi = val;
        set = 7;
        update();
    }

private:
    int slide,warna,set,lebar,tinggi,variable;
    char valueBTR,valueBDI,valuePlts,valuePltmh,valueLoad; //value Overview
    bool valueBTRindi,valueSWT,valueBDIindi; //Switch Overview
    bool valueBTRStat,valueBDIStat,valuePltsStat,valuePltmhStat,valueLoadStat; //status RTU Overview
    char valuePltsInv1,valuePltsInv2,valuePltsInv3,valuePltsOut; //valuew PLTS
    bool valuePltsInv1stat,valuePltsInv2stat,valuePltsInv3stat,valuePltsOutstat; //status RTU PLTS
    char valuePltmhOut; //value PLTMH
    bool valuePltmhOutstat; //status RTU PLTMH
    bool valueLoadRstat,valueLoadSstat,valueLoadTstat; //status RTU Load
    char valueLoadR,valueLoadS,valueLoadT; //value Load
    char valueInvR,valueInvS,valueInvT; //value Inverter
    bool valueInvRstat,valueInvSstat,valueInvTstat; //statusRTU Inverter
    bool valueInvRdir,valueInvSdir,valueInvTdir,valueInvRSw,valueInvSSw,valueInvTSw;  //Switch & dir Inverter
    bool valueBtrOutstat,valueBtrOutindi; //battery status & direction
    char valueBtrOut; //battery value
    void setSmallDonut();
    void setOverview();
    void setPlts();
    void setPltmh();
    void setLoad();
    void setInverter();
    void setBattery();

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DONUT_H
