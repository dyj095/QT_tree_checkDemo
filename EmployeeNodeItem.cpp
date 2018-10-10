#include "EmployeeNodeItem.h"
#include "ui_EmployeeNodeItem.h"

#include <QDebug>
#include <QPainter>

EmployeeNodeItem::EmployeeNodeItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmployeeNodeItem),
    m_headLabelWidth(0),
    m_level(0),
    m_indentation(0),
    m_fullName("")
{
    ui->setupUi(this);
    initControl();
}

EmployeeNodeItem::~EmployeeNodeItem()
{
    delete ui;
}

void EmployeeNodeItem::setFullName(const QString &fullName)
{
    m_fullName = fullName;
    ui->lbFullName->setText(fullName);
}

void EmployeeNodeItem::setHeadPixmap(const QPixmap &headPath)
{
    ui->lbHeadPic->setPixmap(headPath);
}

void EmployeeNodeItem::setHeadPath(const QString &headPath)
{
    /*
    ui->lbHeadPic->setScaledContents(true);
    QString style = ui->lbHeadPic->styleSheet();
    style.append("image:url(").append(headPath).append(");");
    qDebug() << style;
    ui->lbHeadPic->setStyleSheet(style);
    */
    // 方式3.加载QPixmap
    QPixmap pixmap1;
    pixmap1.load(headPath);
    QPixmap pixmap2;
    pixmap2.load(":/tree/Resources//head_mask.png");
    //qDebug() << "m_level:" << m_level << "  m_indentation:" << m_indentation << " m_headLabelWidth:" << m_headLabelWidth << "  " << HEAD_LABEL_WIDTH;
    QPixmap roundPic = this->getRoundImage(pixmap1, pixmap2, QSize(m_headLabelWidth,HEAD_LABEL_WIDTH));
    this->setHeadPixmap(roundPic);
}

QSize EmployeeNodeItem::getHeadLabelSize() const
{
    return ui->lbHeadPic->size();
}

int EmployeeNodeItem::getIndentation()
{
    return this->m_indentation;

}

int EmployeeNodeItem::getLevel()
{
    return this->m_level;
}

void EmployeeNodeItem::setLevel(int level)
{
    this->m_level = level;
    this->m_indentation = this->m_level * INDENTATION;
    this->m_headLabelWidth = this->m_indentation + HEAD_LABEL_WIDTH;
    ui->lbHeadPic->setMinimumWidth(m_indentation);
}

QString EmployeeNodeItem::getFullName()
{
    return m_fullName;
}

void EmployeeNodeItem::initControl()
{

}

QPixmap EmployeeNodeItem::getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize)
{
    if (masksize == QSize(0, 0))
    {
        masksize = mask.size();
    }
    else
    {
        mask = mask.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QImage resultImage(masksize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(m_indentation, 0, mask);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(m_indentation, 0, src.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.end();
    return QPixmap::fromImage(resultImage);
}

void EmployeeNodeItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}
