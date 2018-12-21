#ifndef JCALENDARWIDGET_H
#define JCALENDARWIDGET_H

#include <QCalendarWidget>

class QPushButton;
class QLabel;

class JCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit JCalendarWidget(QWidget *parent = 0);

private:
    void initControl();
    void initTopWidget();
    //void initTimeEditWidget();
    void initBottomWidget();
    void setDataLabelTimeText(int year, int month);

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
    QPushButton* m_leftYearBtn;
    QPushButton* m_leftMonthBtn;

    QPushButton* m_rightYearBtn;
    QPushButton* m_rightMonthBtn;

    QPushButton* m_ensureBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_nowBtn;

    QLabel* m_dataLabel;


signals:
    void sigSetCalendarTime(const QDate& data);
    //void sigClearDateTime();


public slots:
    void slotBtnClicked();


};

#endif // JCALENDARWIDGET_H
