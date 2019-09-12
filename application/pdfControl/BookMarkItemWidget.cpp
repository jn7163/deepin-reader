#include "BookMarkItemWidget.h"
#include <QDebug>


BookMarkItemWidget::BookMarkItemWidget(CustomWidget *parent) :
    CustomWidget ("BookMarkItemWidget", parent)
{

    m_pHLayout = new QHBoxLayout;
    //set around distance
    m_pHLayout->setContentsMargins(1, 0, 1, 0);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slotShowContextMenu(const QPoint &)));

    initWidget();
}

void BookMarkItemWidget::setItemImage(const QImage &image)
{
    m_pPicture->setPixmap(QPixmap::fromImage(image));
}

void BookMarkItemWidget::setPage(const QString &value)
{
    QFont ft;
    ft.setPointSize(12);
    m_pPage->setFont(ft);
    m_pPage->setText(value);
}

int BookMarkItemWidget::dealWithData(const int &, const QString &)
{
    return 0;
}

void BookMarkItemWidget::slotDltBookMark()
{
    //qDebug() << "delet BookMark";
    sendMsg(MSG_BOOKMARK_DLTITEM, QString::number(DLTITEM));
}

void BookMarkItemWidget::slotShowContextMenu(const QPoint &point)
{
    QMenu *t_menu = new QMenu(this);

    QAction *dltBookMarkAction = t_menu->addAction(tr("delete bookmark"));
    connect(dltBookMarkAction, SIGNAL(triggered()), this, SLOT(slotDltBookMark()));
    t_menu->exec(QCursor::pos());
}

void BookMarkItemWidget::initWidget()
{
    m_pPicture = new DLabel;
    m_pPage = new DLabel;

    m_pHLayout->setSpacing(1);
    m_pHLayout->addWidget(m_pPicture);
    m_pHLayout->addWidget(m_pPage);
    m_pHLayout->setSpacing(1);

    this->setLayout(m_pHLayout);
}

