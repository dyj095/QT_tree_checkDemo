#include "DepartNodeItem.h"

#include <QPainter>
#include <QDebug>

DepartNodeItem::DepartNodeItem(QWidget *parent)
    : QLabel(parent),
      m_rotation(0),
      m_level(0),
      m_indentation(0)
{
    setFixedHeight(32);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_animation = new QPropertyAnimation(this, "rotation");
    m_animation->setDuration(50);
    m_animation->setEasingCurve(QEasingCurve::InQuad);
}
DepartNodeItem::~DepartNodeItem()
{
    m_animation = NULL;
    delete m_animation;
}

void DepartNodeItem::setText(const QString &title)
{
    m_name = title;
    update();
}

void DepartNodeItem::setExpanded(bool expand)
{
    if (expand) {
        m_animation->setEndValue(90);
    } else {
        m_animation->setEndValue(0);
    }
    m_animation->start();
}


int DepartNodeItem::getIndentation()
{
    return this->m_indentation;
}

void DepartNodeItem::setLevel(int level)
{
    this->m_level = level;
    this->m_indentation = this->m_level * INDENTATION;
}

int DepartNodeItem::getLevel()
{
    return this->m_level;
}

QString DepartNodeItem::getName()
{
    return this->m_name;
}

int DepartNodeItem::rotation()
{
    return m_rotation;
}

void DepartNodeItem::setRotation(int rotation)
{
    m_rotation = rotation;
    update();
}

void DepartNodeItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    {
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        QFont font;
        font.setPointSize(10);
        painter.setFont(font);
        int txtX = m_indentation + 24;
        painter.drawText(txtX, 0, this->width() - txtX, this->height(), Qt::AlignLeft | Qt::AlignVCenter, m_name);
    }

    {
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.save();

        QPixmap pixmap(":/tree/Resources/arrow.png");
        QPixmap tmpPixmap(pixmap.size());
        tmpPixmap.fill(Qt::transparent);

        QPainter p(&tmpPixmap);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        // 旋转m_rotation角度
        p.translate(pixmap.width() /2, pixmap.height() /2);
        p.rotate(m_rotation);
        p.drawPixmap(0 - pixmap.width() /2, 0 - pixmap.height() / 2,pixmap);
        painter.drawPixmap(m_indentation+6, (this->height() - pixmap.height()) / 2, tmpPixmap);
        painter.restore();
    }
    QLabel::paintEvent(event);
}
