#ifndef DEPARTNODEITEM_H
#define DEPARTNODEITEM_H

#include <QLabel>
#include <QPaintEvent>
#include <QPropertyAnimation>

#define INDENTATION 20

class DepartNodeItem : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
    DepartNodeItem(QWidget *parent = 0);

    ~DepartNodeItem();

    void setText(const QString& title);

    void setExpanded(bool expand);

    int getIndentation();

    void setLevel(int level);

    int getLevel();

private:
    int rotation();

    void setRotation(int rotation);

private:
    void paintEvent(QPaintEvent *event);

private:
    QPropertyAnimation *m_animation;

    QString m_name;

    QString m_id;

    int m_rotation;

    int m_indentation;

    int m_level;
};

#endif // DEPARTNODEITEM_H
