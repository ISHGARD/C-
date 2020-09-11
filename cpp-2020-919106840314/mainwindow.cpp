#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QDebug>

struct conte
{
    double a=0;
    int b;
};

conte cont[ 100 ];
QString show_sc,pre_sc;
int bu_count = 0, sc_count = 0 , ans_count = 0 ,
    ans_point = 0,CD_c = 0,CD_p = 0;                        //数组，字符串，ans，CD计数
int statu = 0 , statu2 = 0;                                 //运算模式
double outc, spe[2] = {0}, spe2[100]={0} ,fun[4]={0};       //输出,值传递
double ans[5] = {0};                                        //ans

QString MainWindow::start_count()
{
    bool i = MainWindow::judge_line1();
    if ( i == 0 ) {  return  QString("ERROR"); }

    trans();

    i = judge_line2();
    if ( i == 0 ) {  return  QString("ERROR"); }

    outc = decisi(0 , bu_count - 1);

    QString ot = QString::number(outc , 'g' , 10);

    ans_save(outc);

    //qDebug() << ans[ans_count] << "  *" <<endl;

    return ot;
}

void MainWindow::ans_save(double a)
{
    ans_count++;
    if(ans_count >= 5) ans_count -= 5;
    ans[ans_count] = a;
    ans_point = ans_count;
}

double MainWindow::decisi(int a , int b)           //计算处理
{
    int x = 0, y = 0,kk = 0;
    bool jud = 0,ju = 0;

    while(!ju)
    {
        for( int i = a ; i <= b ; i++ )                //有无最外圈（）
        {
            if(cont[i].b == 4)
            {
                if( jud == 0)
                {
                    x=i+1;
                    jud = 1;
                }
                kk++;
            }
            if( cont[i].b == 5)
            {
                kk--;
                if(kk == 0 )
                {
                    y = i - 1;
                    break;
                }
            }
        }
        if( jud == 1 )
        {
            decisi( x , y );
            cont[x-1].b = 0;
            cont[y+1].b = 0;
        }
        ju = 1;
        jud = 0;
        for( int i = a ; i <= b ; i++ )
        {
            if(cont[i].b == 4)
            {
                ju = 0;
                break;
            }
        }
    }

    for(int i = a ; i <= b ; i++ )                 //.处理
    {
        //double tem;
        int l;

        if( cont[i].b == 0) continue;
        if( cont[i].b == 1)
        {
            l = i;
            //tem = cont[i].a;
        }

        if ( cont[i].b == 2 && cont[i].a == 6)
        {
            int te = cont[i+1].a;
            //qDebug() << cont[i+1].a <<endl;
            int k;
            for(k = 0; te > 0 ; k++)
                te/=10;
            //qDebug() << l <<endl;
            for( ; k>0 ; k--)
                cont[i+1].a /= 10;
            cont[l].a += cont[i+1].a;
            cont[i+1].b = 0;
            cont[i].b = 0;
           // qDebug() << cont[l].a <<endl;
        }
    }

    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;

    //qDebug() << "---" <<endl;
    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;

    int l = -1;
    for(int i = a ; i <= b ; i++ )                 //Ans,Pi和e
    {
        //double tem;

        if( cont[i].b == 0) continue;

        if( cont[i].b == 1)
        {
            l = i;
            //tem = cont[i].a;
        }
        //qDebug() << cont[l].a <<endl;

        if( cont[i].b > 1 && cont[i].b < 6)
            l = -1;

        if( cont[i].b == 6)
        {
            if( l != -1)
            {
                if( cont[i].a == 1) cont[l].a *= M_PI;
                if( cont[i].a == 2) cont[l].a *= exp(1);
                if( cont[i].a == 3) cont[l].a *= ans[ans_count];
                cont[i].b = 0;
                //qDebug() << cont[l].a <<endl;
            }
            if( l == -1 )
            {
                if( cont[i].a == 1)
                {
                    cont[i].b = 1;
                    cont[i].a = M_PI;
                }
                if( cont[i].a == 2)
                {
                    cont[i].b = 1;
                    cont[i].a = exp(1);
                }
                if( cont[i].a == 3)
                {
                    cont[i].b = 1;
                    cont[i].a = ans[ans_count];
                }
                //qDebug() << cont[i].a <<endl;
            }
        }
    }

    //qDebug() << "---" <<endl;
    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;

    for(int i = a ; i <= b ; i++ )                 //^处理
    {
        double tem ;
        int l;

        if( cont[i].b == 0) continue;
        if( cont[i].b == 1)
        {
            l = i;
            tem = cont[i].a;
        }

        if( cont[i].b == 2 && cont[i].a == 5)
        {
            for(int k = 1 ; k < cont[i+1].a ; k++ )
                cont[l].a *= tem;
            cont[i].b = 0;
            cont[i+1].b = 0;
            //qDebug() << cont[l].a <<endl;
        }
    }

    for(int i = b ; i >= a ; i-- )                 //左单值运算符
    {
        int l;
        //double tem;

        if( cont[i].b == 0) continue;

        if( cont[i].b == 1)
        {
            l = i;
            //tem = cont[i].a;
        }

        if( cont[i].b == 3)
        {
            if(cont[i].a == 1)                       //sqrt
            {
                cont[i].a = sqrt(cont[l].a);
                cont[i].b = 1;
                cont[l].b = 0;
                continue;
            }
            if(cont[i].a == 2)                       //sin
            {
                cont[i].a = sin(cont[l].a);
                if(abs(cont[i].a) < 0.00000001) cont[i].a = 0;
                cont[i].b = 1;
                cont[l].b = 0;
                continue;
            }
            if(cont[i].a == 3)                       //cos
            {
                cont[i].a = cos(cont[l].a);
                if(abs(cont[i].a) < 0.00000001) cont[i].a = 0;
                cont[i].b = 1;
                cont[l].b = 0;
                continue;
            }
            if(cont[i].a == 4)                       //tan
            {
                cont[i].a = tan(cont[l].a);
                if(abs(cont[i].a) < 0.00000001) cont[i].a = 0;
                cont[i].b = 1;
                cont[l].b = 0;
                continue;
            }
            if(cont[i].a == 5)                       //lg
            {
                cont[i].a = log10(cont[l].a);
                cont[i].b = 1;
                cont[l].b = 0;
                continue;
            }
            if(cont[i].a == 6)                       //ln
            {
                cont[i].a = log(cont[l].a);
                cont[i].b = 1;
                cont[l].b = 0;
                continue;
            }
            for( int k = i-1 ; k >= a ; k-- )
            {
                if(cont[k].b == 0) continue;

                if(cont[k].b > 1 ) break;

                if(cont[k].b == 1)
                {
                    cont[i].a *= cont[k].a;
                    cont[k].b = 0;
                }
            }
            //qDebug() << cont[i].a <<endl;
            i++;
        }
        //qDebug() << cont[i].a <<endl;

    }

    //qDebug() << "---" <<endl;
    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;
    //qDebug() << "---" <<endl;

    int sta = 0;
    for(int i = a ; i <= b ; i++ )                 //负号
    {

        if( cont[i].b == 0 ) continue;

        if(sta == 0 && !(cont[i].b == 2 && cont[i].a == 2)) break;

        if( cont[i].b == 2 && cont[i].a == 2 && sta == 0)
        {
            l = i;
            sta = 1;
            continue;
        }

        if( cont[i].b == 1 && sta == 1)
        {
            cont[i].a = 0 - cont[i].a;
            cont[l].b = 0;
            //qDebug() << "---11" <<endl;
            break;
        }

        if(sta == 1 && cont[i].b != 0)
        {
            //qDebug() << "---22" <<endl;
            break;
        }
    }

    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;
    //qDebug() << "---" <<endl;


    for(int i = a ; i <= b ; i++ )                 //乘除
    {
        int l1, l2;

        if( cont[i].b == 0) continue;

        if( cont[i].b == 1) l1 = i;

        if( cont[i].b == 2)
        {
            if( cont[i].a == 3)
            {
                for( int k = i+1 ; k <= b ; k++ )
                {
                    if( cont[k].b == 1)
                    {
                        l2 = k;
                        break;
                    }
                }
                cont[i].a = cont[l2].a * cont[l1].a;
                cont[i].b = 1;
                cont[l1].b = 0;
                cont[l2].b = 0;
                i--;
                //qDebug() << cont[i+1].a <<endl;
            }

            else if( cont[i].a == 4)
            {
                for( int k = i+1 ; k <= b ; k++ )
                {
                    if( cont[k].b == 1)
                    {
                        l2 = k;
                        break;
                    }
                }
                cont[i].a = cont[l1].a / cont[l2].a;
                cont[i].b = 1;
                cont[l1].b = 0;
                cont[l2].b = 0;
                i--;
                //qDebug() << cont[i+1].a <<endl;
            }
        }
        //qDebug() << cont[i+1].a <<endl;
    }

    for(int i = a ; i <= b ; i++ )                 //加减
    {
        int l1 , l2 ;

        if( cont[i].b == 0) continue;

        if( cont[i].b == 1) l1 = i;

        if( cont[i].b == 2)
        {
            if( cont[i].a == 1)
            {
                qDebug() << " " << cont[l1].a <<endl;
                for( int k = i+1 ; k <= b ; k++ )
                {
                    if( cont[k].b == 1)
                    {
                        l2 = k;
                        break;
                    }
                }
                cont[i].a = cont[l2].a + cont[l1].a;
                cont[i].b = 1;
                cont[l1].b = 0;
                cont[l2].b = 0;
                i--;
                //qDebug() << cont[i+1].a <<endl;
            }

            else if( cont[i].a == 2)
            {
                for( int k = i+1 ; k <= b ; k++ )
                {
                    if( cont[k].b == 1)
                    {
                        l2 = k;
                        break;
                    }
                }
                cont[i].a = cont[l1].a - cont[l2].a;
                cont[i].b = 1;
                cont[l1].b = 0;
                cont[l2].b = 0;
                i--;
                //qDebug() << cont[i+1].a <<endl;
            }

        }
       // qDebug() << "---"<<endl;
    }

    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;

    for(int i = a; i <= b ; i++ )
        if( cont[i].b == 1) return cont[i].a ;

    return 0;
}

bool MainWindow::judge_line2()                       //第二次判断
{
    return 1;
}

void MainWindow::trans()                             //字符串转换
{
    int t=0,mat=10;

    for( int i=0 ; i< sc_count ;i++ ,mat =10 ,t=0 )
    {

        if( show_sc[i] == '[' )                                   //非十进制
        {
            for(i++,mat=0 ; show_sc[i] != ']' ; i++)
            {
                mat*=10;
                QChar ch = show_sc.QString::at(i);
                mat+=ch.unicode() - '0';
            }
            //qDebug() << mat << endl;
            i++ ;
        }
        if ( i<= sc_count && ((show_sc[i]>='0' && show_sc[i] <= '9')||(show_sc[i]>= 'A' && show_sc[i] <= 'F' )) )  //数值转换
        {
        for( ; i<= sc_count && ((show_sc[i]>='0' && show_sc[i] <= '9')||(show_sc[i] >= 'A' && show_sc[i] <= 'F' ));i++)
        {

            if(show_sc[i]>='0' && show_sc[i] <= '9')
            {
                t*=mat;
                QChar ch = show_sc.QString::at(i);
                t+=ch.unicode() - '0';
            }
            if( show_sc[i]>= 'A' && show_sc[i] <= 'F')
            {
                t*=mat;
                QChar ch = show_sc.QString::at(i);
                t+=ch.unicode() - 'A'+10;
            }
        }
        i--;
        cont[bu_count].a = t;
        cont[bu_count++].b = 1;
        //qDebug() << t << endl;
        }
        else                                                 //符号对应数值
        {
            if( show_sc[i] == '+'){
                    cont[bu_count].a = 1;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '-'){
                    cont[bu_count].a = 2;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '*'){
                    cont[bu_count].a = 3;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '/'){
                    cont[bu_count].a = 4;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '^'){
                    cont[bu_count].a = 5;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '.'){
                    cont[bu_count].a = 6;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == 's' && show_sc[i+1] == 'q'){   //sqrt
                    cont[bu_count].a = 1;
                    cont[bu_count++].b = 3;
                    i+=3; }
            if( show_sc[i] == 's' && show_sc[i+1] == 'i'){   //sin
                    cont[bu_count].a = 2;
                    cont[bu_count++].b = 3;
                    i+=2; }
            if( show_sc[i] == 'c' && show_sc[i+1] == 'o'){   //cos
                    cont[bu_count].a = 3;
                    cont[bu_count++].b = 3;
                    i+=2; }
            if( show_sc[i] == 't' && show_sc[i+1] == 'a'){   //tan
                    cont[bu_count].a = 4;
                    cont[bu_count++].b = 3;
                    i+=2; }
            if( show_sc[i] == 'l' && show_sc[i+1] == 'g'){   //lg
                    cont[bu_count].a = 5;
                    cont[bu_count++].b = 3;
                    i++;  }
            if( show_sc[i] == 'l' && show_sc[i+1] == 'n'){   //ln
                    cont[bu_count].a = 6;
                    cont[bu_count++].b = 3;
                    i++;  }
            if( show_sc[i] == 'P' && show_sc[i+1] == 'i'){   //Pi
                    cont[bu_count].a = 1;
                    cont[bu_count++].b = 6;
                    i++;  }
            if( show_sc[i] == 'e' ){                         //e
                    cont[bu_count].a = 2;
                    cont[bu_count++].b = 6;  }
            if( show_sc[i] == 'a' && show_sc[i+1] == 'n'){   //ans
                    cont[bu_count].a = 3;
                    cont[bu_count++].b = 6;
                    i+=2; }
            if( show_sc[i] == '('){
                    cont[bu_count++].b = 4;  }
            if( show_sc[i] == ')'){
                    cont[bu_count++].b = 5;  }
        }

    }
    //for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;
}

bool MainWindow::judge_line1()                       //第一次判断
{
    int l1=0,l2=0,r1=0,r2=0;

    for(int i=0 ; i<sc_count ;i++)                  //括号对称
    {
        if(show_sc[i] == '(') l1++;
        if(show_sc[i] == '[') l2++;
        if(show_sc[i] == ')') r1++;
        if(show_sc[i] == ']') r2++;
    }

    if(l1 != r1 || l2 != r2) return 0;

    for(int i=0 ; i<sc_count ;i++)
    {
        if ( show_sc[i] == '[')                      // [进制] 问题
        {
            if ( show_sc[i+1] == ']') return 0;      //[]
            if ( i > 0 && show_sc[i-1] == '.') return 0;      //[x]3.[x]3
            for(i++ ; show_sc[i] != ']'; i++)        //[+-^]
            {
                if(show_sc[i] < '0' || show_sc[i] > '9') return 0;
            }
        }
    }
    return 1;
}

void MainWindow::CD(double a, double b)              //公约数
{
    //qDebug() << "******";
    int x = a, y = b , z ;
    z = x>y?x/2:y/2;
    for(int i = 1 ; i<=z ; i++)
        if(x%i == 0 && y%i == 0) spe2[CD_c++] = i;
    CD_p = CD_c - 1;
    //for( int i=0; i < CD_c ; i++) qDebug() << spe2[i];
}

int MainWindow::LCM(double a, double b)           //最小公倍数
{
    int x = a , y = b ,z , w;
    z= x>y?x:y;
    w = x* y ;
    for(int i = z ; i <= w ; i++ )
        if(i%x == 0 && i%y == 0)
            return i;
    return 0;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //button settings
    connect(ui->pushButton_55, &QPushButton::clicked, [=] () mutable {     //AC
        show_sc = "0";
        pre_sc = "0";
        sc_count = 0;
        statu = 0;
        statu2 = 0;
        bu_count = 0;
        CD_c = 0;
        CD_p = 0;
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_31, &QPushButton::clicked, [=] () mutable {     //CMP
        sc_count = 0;
        statu = 1;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " first num:";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_32, &QPushButton::clicked, [=] () mutable {     //CD
        sc_count = 0;
        statu = 2;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " first num:";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_33, &QPushButton::clicked, [=] () mutable {     //LCM
        sc_count = 0;
        statu = 3;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " first num:";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_35, &QPushButton::clicked, [=] () mutable {     //ax+b
        sc_count = 0;
        statu = 4;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " a=";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_36, &QPushButton::clicked, [=] () mutable {     //a/x
        sc_count = 0;
        statu = 5;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " a=";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_37, &QPushButton::clicked, [=] () mutable {     //ax^2+bx+c
        sc_count = 0;
        statu = 6;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " a=";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_38, &QPushButton::clicked, [=] () mutable {     //a^x
        sc_count = 0;
        statu = 7;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " a=";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_39, &QPushButton::clicked, [=] () mutable {     //loga x
        sc_count = 0;
        statu = 8;
        statu2 = 0;
        bu_count = 0;
        show_sc = "0";
        pre_sc = " a=";
        ui->label->setText(QString(pre_sc));
        ui->lineEdit->setText(QString(show_sc));
    }  );

    if ( sc_count <= 90 ) {
        connect(ui->pushButton_54, &QPushButton::clicked, [=] () mutable {     //一般DEL
            if( sc_count > 0)
            {
                sc_count--;
                show_sc [ sc_count ] = 0;
                ui->lineEdit->setText(QString(show_sc));
            }
        }  );
    connect(ui->pushButton_0, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '0';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_1, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '1';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_2, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '2';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_3, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '3';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_4, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '4';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_5, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '5';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_6, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '6';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_7, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '7';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_8, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '8';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_9, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '9';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_10, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'A';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_11, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'B';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_12, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'C';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_13, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'D';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_14, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'E';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_15, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'F';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_16, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '.';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_51, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'P';
        show_sc [ sc_count++ ] = 'i';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_17, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '+';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_18, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '-';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_19, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '*';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_20, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '/';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_21, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '^';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_22, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 's';
        show_sc [ sc_count++ ] = 'q';
        show_sc [ sc_count++ ] = 'r';
        show_sc [ sc_count++ ] = 't';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_23, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 's';
        show_sc [ sc_count++ ] = 'i';
        show_sc [ sc_count++ ] = 'n';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_24, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'c';
        show_sc [ sc_count++ ] = 'o';
        show_sc [ sc_count++ ] = 's';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_25, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 't';
        show_sc [ sc_count++ ] = 'a';
        show_sc [ sc_count++ ] = 'n';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_26, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '(';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_27, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = ')';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_28, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'l';
        show_sc [ sc_count++ ] = 'n';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_29, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'l';
        show_sc [ sc_count++ ] = 'g';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_34, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'e';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_41, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '[';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_42, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = ']';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_52, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '-';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_53, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'a';
        show_sc [ sc_count++ ] = 'n';
        show_sc [ sc_count++ ] = 's';
        ui->lineEdit->setText(QString(show_sc));
    } );
     //--------------------------
    }

    connect(ui->pushButton_50, &QPushButton::clicked, [=] () mutable {
        if(statu == 0)                                                       //一般=
        {
            ui->label->setText(QString(show_sc));
            QString ot = MainWindow::start_count ();
            ui->lineEdit->setText(ot);
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
        }
        if(statu == 1)                                                        //CMP =
        {
            QString ot = start_count();
            pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " second num:";
            ui->label->setText(QString(pre_sc));
            spe[statu2++] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 == 1) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 2)
            {
                if(spe[0] > spe[1]) ui->lineEdit->setText(QString(">"));
                if(spe[0] == spe[1]) ui->lineEdit->setText(QString("="));
                if(spe[0] < spe[1]) ui->lineEdit->setText(QString("<"));

                statu = 0;
                pre_sc = "0";
                statu2 = 0;
            }
        }
        if(statu == 2)                                                       // CD =
        {
            QString ot = start_count();
            pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " second num:";
            ui->label->setText(QString(pre_sc));
            spe[statu2++] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 == 1) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 2)
            {
                CD(spe[0] , spe[1]);
                QString ott = QString::number(spe2[CD_c-1] , 'g' , 10);
                ui->lineEdit->setText(QString(ott));
            }
        }
        if(statu == 3)                                                        //LCM =
        {
            QString ot = start_count();
            pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " second num:";
            ui->label->setText(QString(pre_sc));
            spe[statu2++] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 == 1) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 2)
            {
                double ot = LCM( spe[0] , spe [1]);
                QString ott = QString::number(ot , 'g' , 10);
                ui->lineEdit->setText(QString(ott));

                statu = 0;
                pre_sc = "0";
                statu2 = 0;
            }
        }
        if(statu == 4)                                                        //ax+b =
        {
            QString ot = start_count();
            if (statu2 <= 1) pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " b=";
            if (statu2 == 1) pre_sc = pre_sc + " x=";
            ui->label->setText(QString(pre_sc));
            fun[statu2] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 <= 1) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 2)
            {
                double ot = fun[0]*fun[2]+fun[1];
                QString ott = QString::number(ot , 'g' , 10);
                ui->lineEdit->setText(QString(ott));
            }
            if ( statu2 <= 1 ) statu2++;
        }
        if(statu == 5)                                                        //a/x =
        {
            QString ot = start_count();
            if (statu2 == 0) pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " x=";
            ui->label->setText(QString(pre_sc));
            fun[statu2] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 == 0) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 1)
            {
                double ot = fun[0]/fun[1];
                QString ott = QString::number(ot , 'g' , 10);
                ui->lineEdit->setText(QString(ott));
            }
            if ( statu2 == 0 ) statu2++;
        }
        if(statu == 6)                                                        //ax^2+bx+c =
        {
            QString ot = start_count();
            if (statu2 <= 2) pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " b=";
            if (statu2 == 1) pre_sc = pre_sc + " c=";
            if (statu2 == 2) pre_sc = pre_sc + " x=";
            ui->label->setText(QString(pre_sc));
            fun[statu2] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 <= 2) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 3)
            {
                double ot = fun[0]*fun[3]*fun[3]+fun[1]*fun[3]+fun[2];
                QString ott = QString::number(ot , 'g' , 10);
                ui->lineEdit->setText(QString(ott));
            }
            if ( statu2 <= 2 ) statu2++;
        }
        if(statu == 7)                                                        //a^x =
        {
            QString ot = start_count();
            if (statu2 == 0) pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " x=";
            ui->label->setText(QString(pre_sc));
            fun[statu2] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 == 0) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 1)
            {
                double ot = 1;
                int x1 = fun[1];
                for(int i = 0 ; i < x1 ; i++ )
                    ot*= fun[0];
                QString ott = QString::number(ot , 'g' , 10);
                ui->lineEdit->setText(QString(ott));
            }
            if ( statu2 == 0 ) statu2++;
        }
        if(statu == 8)                                                        //loga x =
        {
            QString ot = start_count();
            if (statu2 == 0) pre_sc = pre_sc + ot;
            if (statu2 == 0) pre_sc = pre_sc + " x=";
            ui->label->setText(QString(pre_sc));
            fun[statu2] = outc;
            show_sc = "0";
            sc_count = 0;
            bu_count = 0;
            if( statu2 == 0) ui->lineEdit->setText(QString(show_sc));
            if( statu2 == 1)
            {
                double ot = log(fun[1])/log(fun[0]);
                QString ott = QString::number(ot , 'g' , 10);
                ui->lineEdit->setText(QString(ott));
            }
            if ( statu2 == 0 ) statu2++;
        }
    }  );
    connect(ui->pushButton_40, &QPushButton::clicked, [=] () mutable {     //<-
        if( statu == 0)
        {
            QString temm = QString::number(ans[ans_point] , 'g' , 10);
            ui->label->setText(QString(temm));
            ans_point--;
            if( ans_point < 0) ans_point +=5 ;
        }
        if(statu == 2)
        {
            CD_p--;
            if( CD_p < 0) CD_p = CD_c -1;
            QString ott = QString::number(spe2[CD_p] , 'g' , 10);
            ui->lineEdit->setText(QString(ott));
        }
    }  );
    connect(ui->pushButton_43, &QPushButton::clicked, [=] () mutable {     //->
        if( statu == 0)
        {
            QString temm = QString::number(ans[ans_point] , 'g' , 10);
            ui->label->setText(QString(temm));
            ans_point++;
            if( ans_point >= 5) ans_point -=5 ;
        }
        if(statu == 2)
        {
            CD_p++;
            if( CD_p == CD_c) CD_p = 0;
            QString ott = QString::number(spe2[CD_p] , 'g' , 10);
            ui->lineEdit->setText(QString(ott));
        }
    }  );
    connect(ui->pushButton_48, &QPushButton::clicked, [=] () mutable {     //Cle an
        for(int g=0 ; g<5 ; g++) ans[g] = 0;
    }  );
}


MainWindow::~MainWindow ()
    {
        delete ui;
    }










