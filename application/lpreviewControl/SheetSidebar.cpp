#include "SheetSidebar.h"
#include "DocSheet.h"
#include "thumbnail/ThumbnailWidget.h"
#include "catalog/CatalogWidget.h"
#include "bookmark/BookMarkWidget.h"
#include "note/NotesWidget.h"
#include "search/SearchResWidget.h"
#include "utils/PublicFunction.h"

#include <QButtonGroup>
#include <QVBoxLayout>

SheetSidebar::SheetSidebar(DocSheet *parent, PreviewWidgesFlags widgesFlag)
    : CustomWidget(parent)
    , m_sheet(parent)
    , m_widgetsFlag(widgesFlag | PREVIEW_SEARCH)
{
    initWidget();
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &SheetSidebar::onUpdateWidgetTheme);
}

SheetSidebar::~SheetSidebar()
{

}

void SheetSidebar::initWidget()
{
    m_scale           = 1.0;
    m_bOldVisible     = false;
    m_thumbnailWidget = nullptr;
    m_catalogWidget   = nullptr;
    m_bookmarkWidget  = nullptr;
    m_notesWidget     = nullptr;
    m_searchWidget    = nullptr;

    int tW = LEFTMINWIDTH;
    int tH = LEFTMINWIDTH;
    dApp->adaptScreenView(tW, tH);
    setMinimumWidth(tW);

    tW = LEFTMAXWIDTH;
    dApp->adaptScreenView(tW, tH);
    setMaximumWidth(tW);

    tW = LEFTNORMALWIDTH;
    dApp->adaptScreenView(tW, tH);
    resize(tW, this->height());

    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setContentsMargins(0, 0, 0, 0);
    pVBoxLayout->setSpacing(0);
    this->setLayout(pVBoxLayout);

    m_stackLayout = new QStackedLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(15, 0, 15, 0);

    m_btnGroup = new QButtonGroup(this);
    connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(onBtnClicked(int)));
    if (m_widgetsFlag.testFlag(PREVIEW_THUMBNAIL)) {
        m_thumbnailWidget = new ThumbnailWidget(m_sheet, this);
        int index = m_stackLayout->addWidget(m_thumbnailWidget);
        DToolButton *btn = createBtn(tr("Thumbnails"), "thumbnail");
        m_btnGroup->addButton(btn, index);
        hLayout->addWidget(btn);
        hLayout->addStretch();
    }

    if (m_widgetsFlag.testFlag(PREVIEW_CATALOG)) {
        m_catalogWidget = new CatalogWidget(m_sheet, this);
        int index = m_stackLayout->addWidget(m_catalogWidget);
        DToolButton *btn = createBtn(tr("Catalog"), "catalog");
        m_btnGroup->addButton(btn, index);
        hLayout->addWidget(btn);
        hLayout->addStretch();
    }

    if (m_widgetsFlag.testFlag(PREVIEW_BOOKMARK)) {
        m_bookmarkWidget = new BookMarkWidget(m_sheet, this);
        if (m_thumbnailWidget) connect(m_bookmarkWidget, SIGNAL(sigSetBookMarkState(const int &, const int &)), m_thumbnailWidget, SLOT(onSetBookMarkState(const int &, const int &)));
        int index = m_stackLayout->addWidget(m_bookmarkWidget);
        DToolButton *btn = createBtn(tr("Bookmarks"), "bookmark");
        m_btnGroup->addButton(btn, index);
        hLayout->addWidget(btn);
        hLayout->addStretch();
    }

    if (m_widgetsFlag.testFlag(PREVIEW_NOTE)) {
        m_notesWidget = new NotesWidget(m_sheet, this);
        connect(m_notesWidget, SIGNAL(sigDeleteContent(const int &, const QString &)), this, SIGNAL(sigDeleteAnntation(const int &, const QString &)));
        int index = m_stackLayout->addWidget(m_notesWidget);
        DToolButton *btn = createBtn(tr("Annotations"), "annotation");
        m_btnGroup->addButton(btn, index);
        hLayout->addWidget(btn);
        hLayout->addStretch();
    }

    //remove last spaceitem
    QLayoutItem *item = hLayout->takeAt(hLayout->count() - 1);
    if (item) delete item;

    if (m_widgetsFlag.testFlag(PREVIEW_SEARCH)) {
        m_searchWidget = new SearchResWidget(m_sheet, this);
        m_searchId = m_stackLayout->addWidget(m_searchWidget);
        DToolButton *btn = createBtn("Search", "search");
        btn->setVisible(false);
        m_btnGroup->addButton(btn, m_searchId);
    }

    pVBoxLayout->addLayout(m_stackLayout);
    pVBoxLayout->addLayout(hLayout);
    onUpdateWidgetTheme();
    this->setVisible(false);
}

void SheetSidebar::onBtnClicked(int index)
{
    m_btnGroup->button(index)->setChecked(true);
    m_stackLayout->setCurrentIndex(index);
    adaptWindowSize(m_scale);
    if (m_stackLayout->currentWidget() != m_searchWidget)
        m_sheet->setOper(LeftIndex, QString::number(index));
}

void SheetSidebar::setBookMark(int page, int state)
{
    if (m_bookmarkWidget) {
        m_bookmarkWidget->handleBookMark(page, state);
    }
}

void SheetSidebar::handleOpenSuccess()
{
    this->setVisible(m_sheet->getOper(Thumbnail).toInt());
    int nId = qBound(0, m_sheet->getOper(LeftIndex).toInt(), m_stackLayout->count() - 2);

    QAbstractButton *btn = m_btnGroup->button(nId);
    if (btn) m_btnGroup->buttonClicked(nId);
    if (m_thumbnailWidget) m_thumbnailWidget->handleOpenSuccess();
    if (m_catalogWidget) m_catalogWidget->handleOpenSuccess();
    if (m_bookmarkWidget) m_bookmarkWidget->handleOpenSuccess();
    if (m_notesWidget) m_notesWidget->handleOpenSuccess();
}

void SheetSidebar::handleFindOperation(int type)
{
    if (type == E_FIND_CONTENT) {
        const QList<QAbstractButton *> &btns = m_btnGroup->buttons();
        for (QAbstractButton *btn : btns) btn->setEnabled(false);

        m_searchWidget->clearFindResult();
        QAbstractButton *btn = m_btnGroup->button(m_searchId);
        if (btn) m_btnGroup->buttonClicked(m_searchId);
        this->setVisible(true);
    } else if (type == E_FIND_EXIT) {
        const QList<QAbstractButton *> &btns = m_btnGroup->buttons();
        for (QAbstractButton *btn : btns) btn->setEnabled(true);

        int nId = qBound(0, m_sheet->getOper(LeftIndex).toInt(), m_stackLayout->count() - 1);
        QAbstractButton *btn = m_btnGroup->button(nId);
        if (btn) m_btnGroup->buttonClicked(nId);
        this->setVisible(m_sheet->getOper(Thumbnail).toInt());
        m_searchWidget->clearFindResult();
    }
}

void SheetSidebar::handleFindContentComming(const stSearchRes &res)
{
    m_searchWidget->handFindContentComming(res);
}

int SheetSidebar::handleFindFinished()
{
    return m_searchWidget->handleFindFinished();
}

void SheetSidebar::handleRotate(int rotate)
{
    if (m_thumbnailWidget) m_thumbnailWidget->handleRotate(rotate);
}

void SheetSidebar::handleUpdateThumbnail(const int &page)
{
    CustomWidget *curWidget = dynamic_cast<CustomWidget *>(m_stackLayout->currentWidget());
    if (curWidget) curWidget->updateThumbnail(page);
}

void SheetSidebar::handleAnntationMsg(const int &msg, const QString &text)
{
    if (m_notesWidget) m_notesWidget->handleAnntationMsg(msg, text);
}

void SheetSidebar::onPageChanged(int page)
{
    if (m_thumbnailWidget) m_thumbnailWidget->handlePage(page);
    if (m_bookmarkWidget) m_bookmarkWidget->handlePage(page);
    if (m_catalogWidget) m_catalogWidget->handlePage(page);
}

DToolButton *SheetSidebar::createBtn(const QString &btnName, const QString &objName)
{
    int tW = 36;
    auto btn = new DToolButton(this);
    btn->setToolTip(btnName);
    btn->setObjectName(objName);
    btn->setFixedSize(QSize(tW, tW));
    btn->setIconSize(QSize(tW, tW));
    btn->setCheckable(true);
    return btn;
}

void SheetSidebar::resizeEvent(QResizeEvent *event)
{
    qreal scale = event->size().width() * 1.0 / LEFTMINWIDTH;
    adaptWindowSize(scale);
    CustomWidget::resizeEvent(event);
}

void SheetSidebar::adaptWindowSize(const double &scale)
{
    m_scale = scale;
    CustomWidget *curWidget = dynamic_cast<CustomWidget *>(m_stackLayout->currentWidget());
    if (curWidget) curWidget->adaptWindowSize(scale);
}

void SheetSidebar::keyPressEvent(QKeyEvent *event)
{
    QStringList pFilterList = QStringList() << KeyStr::g_pgup << KeyStr::g_pgdown
                              << KeyStr::g_down << KeyStr::g_up
                              << KeyStr::g_left << KeyStr::g_right
                              << KeyStr::g_del;
    QString key = Utils::getKeyshortcut(event);
    if (pFilterList.contains(key)) {
        dealWithPressKey(key);
    }
    CustomWidget::keyPressEvent(event);
}

void SheetSidebar::dealWithPressKey(const QString &sKey)
{
    if (sKey == KeyStr::g_up || sKey == KeyStr::g_pgup || sKey == KeyStr::g_left) {
        onJumpToPrevPage();
    } else if (sKey == KeyStr::g_down || sKey == KeyStr::g_pgdown || sKey == KeyStr::g_right) {
        onJumpToNextPage();
    } else if (sKey == KeyStr::g_del) {
        deleteItemByKey();
    }
}

void SheetSidebar::handleLeftPressKey()
{
    onJumpToPrevPage();
}

void SheetSidebar::handleRightPressKey()
{
    onJumpToNextPage();
}

void SheetSidebar::onJumpToPrevPage()
{
    QWidget *curWidget = m_stackLayout->currentWidget();
    if (curWidget == m_thumbnailWidget) {
        m_thumbnailWidget->prevPage();
    }  else if (curWidget == m_bookmarkWidget) {
        m_bookmarkWidget->prevPage();
    } else if (curWidget == m_notesWidget) {
        m_notesWidget->prevPage();
    }
}

void SheetSidebar::onJumpToNextPage()
{
    QWidget *curWidget = m_stackLayout->currentWidget();
    if (curWidget == m_thumbnailWidget) {
        m_thumbnailWidget->nextPage();
    }  else if (curWidget == m_bookmarkWidget) {
        m_bookmarkWidget->nextPage();
    } else if (curWidget == m_notesWidget) {
        m_notesWidget->nextPage();
    }
}

void SheetSidebar::deleteItemByKey()
{
    QWidget *widget = m_stackLayout->currentWidget();
    if (widget == m_bookmarkWidget) {
        m_bookmarkWidget->DeleteItemByKey();
    } else if (widget == m_notesWidget) {
        m_notesWidget->DeleteItemByKey();
    }
}

void SheetSidebar::onUpdateWidgetTheme()
{
    updateWidgetTheme();
    const QList<QAbstractButton *> &btns = m_btnGroup->buttons();
    for (QAbstractButton *btn : btns) {
        const QString &objName = btn->objectName();
        const QIcon &icon = PF::getIcon(Pri::g_module + objName);
        btn->setIcon(icon);
    }
}
