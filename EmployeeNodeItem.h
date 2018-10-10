#ifndef EMPLOYEENODEITEM_H
#define EMPLOYEENODEITEM_H

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <QSize>

#define INDENTATION 20
#define HEAD_LABEL_WIDTH 32

namespace Ui {
class EmployeeNodeItem;
}

class EmployeeNodeItem : public QWidget
{
    Q_OBJECT

public:
    EmployeeNodeItem(QWidget *parent = 0);
    ~EmployeeNodeItem();

public:
    void setFullName(const QString& fullName);

    void setHeadPixmap(const QPixmap& headPath);

    void setHeadPath(const QString& headPath);

    QSize getHeadLabelSize() const;

    int getIndentation();

    int getLevel();

    void setLevel(int level);

    QString getFullName();

private:
    void initControl();
    QPixmap getRoundImage(const QPixmap &src, QPixmap& mask, QSize masksize);

private:
    void paintEvent(QPaintEvent *event);
private:
    Ui::EmployeeNodeItem *ui;

    int m_indentation;

    int m_level;

    int m_headLabelWidth;
    QString m_fullName;
};

#endif // EMPLOYEENODEITEM_H
