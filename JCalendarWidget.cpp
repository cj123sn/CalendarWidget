#include "JCalendarWidget.h"

#include <QLocale>
#include <QPainter>
#include <QTextCharFormat>
#include <QProxyStyle>
#include <QTableView>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>


class QCustomStyle : public QProxyStyle
{
public:
    QCustomStyle(QWidget *parent){
        setParent(parent);
    }

private:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
        QPainter *painter, const QWidget *widget) const
    {
        if (element == PE_FrameFocusRect)
        {
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

JCalendarWidget::JCalendarWidget(QWidget *parent) : QCalendarWidget(parent)
{
    initControl();
}

void JCalendarWidget::initControl()
{
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setLocale(QLocale(QLocale::Chinese));
    setNavigationBarVisible(false);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    setStyle(new QCustomStyle(this));

    QTextCharFormat format;
    format.setForeground(QColor(160, 160, 160));
    format.setBackground(QColor(255, 255, 255));

    setHeaderTextFormat(format);
    setWeekdayTextFormat(Qt::Saturday, format);
    setWeekdayTextFormat(Qt::Sunday,   format);
    setWeekdayTextFormat(Qt::Monday,   format);
    setWeekdayTextFormat(Qt::Tuesday,  format);
    setWeekdayTextFormat(Qt::Wednesday,format);
    setWeekdayTextFormat(Qt::Thursday, format);
    setWeekdayTextFormat(Qt::Friday,   format);

    initTopWidget();
    initBottomWidget();

    connect(this, &QCalendarWidget::currentPageChanged, [this](int year, int month){
        setDataLabelTimeText(year, month);
    });

    //connect(this, &QCalendarWidget::clicked, this, &QCustomCalendarTimeWidget::onUpdateHMSmmTime);
}

void JCalendarWidget::initTopWidget()
{
    QWidget* topWidget = new QWidget(this);
    topWidget->setObjectName("CalendarTopWidget");
    topWidget->setFixedHeight(40);
    topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    topWidget->setStyleSheet("QWidget#CalendarTopWidget{border-bottom: 1px solid #eeeeee;"
                             "background-color: rgb(255, 255, 255);border-top-left-radius:4px;border-top-right-radius:4px;}");

    QHBoxLayout* hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(12, 0, 12, 0);
    hboxLayout->setSpacing(4);

    m_leftYearBtn   = new QPushButton(this);
    m_leftMonthBtn  = new QPushButton(this);
    m_rightYearBtn  = new QPushButton(this);
    m_rightMonthBtn = new QPushButton(this);
    m_dataLabel     = new QLabel(this);

    m_leftYearBtn->setObjectName("CalendarLeftYearBtn");
    m_leftMonthBtn->setObjectName("CalendarLeftMonthBtn");
    m_rightYearBtn->setObjectName("CalendarRightYearBtn");
    m_rightMonthBtn->setObjectName("CalendarRightMonthBtn");
    m_dataLabel->setObjectName("CalendarDataLabel");

    m_leftYearBtn->setStyleSheet("QPushButton#CalendarLeftYearBtn{border-image:url(:/png/previousyear.png);}"
                                 "QPushButton#CalendarLeftYearBtn:pressed{border-image:url(:/png/previousyear-hover.png);}");

    m_leftMonthBtn->setStyleSheet("QPushButton#CalendarLeftMonthBtn{border-image:url(:/png/previousmonth.png);}"
                                  "QPushButton#CalendarLeftMonthBtn:pressed{border-image:url(:/png/previous-hover.png);}");


    m_rightYearBtn->setStyleSheet("QPushButton#CalendarRightYearBtn:pressed{border-image:url(:/png/nextyear-hover.png);}"
                                  "QPushButton#CalendarRightYearBtn{border-image:url(:/png/nextyear.png);}");

    m_rightMonthBtn->setStyleSheet("QPushButton#CalendarRightMonthBtn:pressed{border-image:url(:/png/nextmonth-hover.png);}"
                                   "QPushButton#CalendarRightMonthBtn{border-image:url(:/png/nextmonth.png);}");

    m_dataLabel->setStyleSheet("QLabel#CalendarDataLabel{font-size:12px;color#666666}");

    m_leftYearBtn->setFixedSize(16, 16);
    m_leftMonthBtn->setFixedSize(16, 16);
    m_rightYearBtn->setFixedSize(16, 16);
    m_rightMonthBtn->setFixedSize(16, 16);

    hboxLayout->addWidget(m_leftYearBtn);
    hboxLayout->addWidget(m_leftMonthBtn);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_dataLabel);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_rightMonthBtn);
    hboxLayout->addWidget(m_rightYearBtn);
    topWidget->setLayout(hboxLayout);

    QVBoxLayout *vBodyLayout = qobject_cast<QVBoxLayout *>(layout());
    vBodyLayout->insertWidget(0, topWidget);

    connect(m_leftYearBtn,   SIGNAL(clicked()),  this, SLOT(slotBtnClicked()));
    connect(m_leftMonthBtn,  SIGNAL(clicked()),  this, SLOT(slotBtnClicked()));
    connect(m_rightYearBtn,  SIGNAL(clicked()),  this, SLOT(slotBtnClicked()));
    connect(m_rightMonthBtn, SIGNAL(clicked()),  this, SLOT(slotBtnClicked()));

    setDataLabelTimeText(selectedDate().year(), selectedDate().month());
}

void JCalendarWidget::initBottomWidget()
{
    QWidget* bottomWidget = new QWidget(this);
    bottomWidget->setObjectName("CalendarBottomWidget");
    bottomWidget->setFixedHeight(40);
    bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    bottomWidget->setStyleSheet("QWidget#CalendarBottomWidget{border-top: 1px solid #eeeeee;background-color: rgb(255, 255, 255);"
                                "border-bottom-left-radius:4px;border-bottom-right-radius:4px;}");

    QHBoxLayout* hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(12, 0, 12, 0);
    hboxLayout->setSpacing(5);

// 	m_nowBtn = new QPushButton(this);
// 	m_nowBtn->setObjectName("CalendarNowBtn");
// 	m_nowBtn->setFixedSize(40, 22);
// 	m_nowBtn->setText(QStringLiteral("现在"));

    m_ensureBtn = new QPushButton(this);
    m_ensureBtn->setObjectName("CalendarEnsureBtn");
    m_ensureBtn->setFixedSize(40, 22);
    m_ensureBtn->setText(QStringLiteral("确定"));
    m_ensureBtn->setStyleSheet("QPushButton#CalendarEnsureBtn{background: #00A1FF;border-radius: 4px;font-size: 12px;color: #FFFFFF;}");

//    m_clearBtn = new QPushButton(this);
//    m_clearBtn->hide();         // 有点多余,先隐藏清空
//    m_clearBtn->setObjectName("CalendarClearBtn");
//    m_clearBtn->setFixedSize(40, 22);
//    m_clearBtn->setText(QStringLiteral("清空"));
//    m_clearBtn->setStyleSheet("QPushButton#CalendarClearBtn{background:#00A1FF;border-radius: 4px;font-size: 12px;color:#FFFFFF;}");

    m_nowBtn = new QPushButton(this);
    m_nowBtn->setObjectName("CalendarNowBtn");
    m_nowBtn->setFixedSize(40, 22);
    m_nowBtn->setText(QStringLiteral("现在"));
    m_nowBtn->setStyleSheet("QPushButton#CalendarNowBtn{background:#00A1FF;border-radius: 4px;font-size: 12px;color:#FFFFFF;}");


    hboxLayout->addStretch();
    hboxLayout->addWidget(m_nowBtn);
    //hboxLayout->addWidget(m_clearBtn);
    hboxLayout->addWidget(m_ensureBtn);
    bottomWidget->setLayout(hboxLayout);

    QVBoxLayout *vBodyLayout = qobject_cast<QVBoxLayout *>(layout());
    vBodyLayout->addWidget(bottomWidget);

    connect(m_ensureBtn, &QPushButton::clicked, [this](){
//        QTime time;
//        QDateTime timeDate;
//        time.setHMS(m_hhTimeeditEdit->text().toInt(), m_mmTimeeditEdit->text().toInt(), m_ssTimeeditEdit->text().toInt());
//        timeDate.setDate(selectedDate());
//        timeDate.setTime(time);
        emit activated(selectedDate());
        emit sigSetCalendarTime(selectedDate());
    });

    connect(m_nowBtn, &QPushButton::clicked, [this](){
        showToday();
    });

//    connect(m_clearBtn, &QPushButton::clicked, [this](){
//        emit sigClearDateTime();
//    });
}

void JCalendarWidget::setDataLabelTimeText(int year, int month)
{
    m_dataLabel->setText(QStringLiteral("%1年%2月").arg(year).arg(month));
}

void JCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if (date == selectedDate())
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 145, 255));

        painter->drawRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6);
        painter->setPen(QColor(255, 255, 255));

        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }
    else if (date < minimumDate() || date > maximumDate())
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(249, 249, 249));

        painter->drawRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6);
        painter->setPen(QColor(220, 220, 220));

        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }
    else if (date == QDate::currentDate())
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 161, 255));
        painter->drawRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6);
        painter->setBrush(QColor(255, 255, 255));
        painter->drawRect(rect.x() + 1, rect.y() + 4, rect.width() - 2, rect.height() - 8);
        painter->setPen(QColor(0, 161, 255));

        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }
    else
    {
        QCalendarWidget::paintCell(painter, rect, date);
    }
}

void JCalendarWidget::slotBtnClicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    if (senderBtn == m_leftYearBtn)
    {
        showPreviousYear();
    }
    else if (senderBtn == m_leftMonthBtn)
    {
        showPreviousMonth();
    }
    else if (senderBtn == m_rightYearBtn)
    {
        showNextYear();
    }
    else if (senderBtn == m_rightMonthBtn)
    {
        showNextMonth();
    }
}

