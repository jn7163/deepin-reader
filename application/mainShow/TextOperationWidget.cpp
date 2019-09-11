#include "TextOperationWidget.h"
#include <DPushButton>

TextOperationWidget::TextOperationWidget(CustomWidget *parent)
    : CustomWidget("TextOperationWidget", parent)
{
    setWindowFlags(Qt::Popup);

    initWidget();
}

void TextOperationWidget::createBtn(const QString &btnName, const char *member)
{
    DPushButton *btn = new DPushButton(btnName);
    connect(btn, SIGNAL(clicked()), member);

    layout->addWidget(btn);
}

void TextOperationWidget::SlotBtnCopyClicked()
{
    sendMsgAndHide(MSG_OPERATION_TEXT_COPY);
}

void TextOperationWidget::SlotBtnRemoveHighLightedClicked()
{
    sendMsgAndHide(MSG_OPERATION_TEXT_REMOVE_HIGHLIGHTED);
}

void TextOperationWidget::SlotBtnAddAnnotationClicked()
{
    sendMsgAndHide(MSG_OPERATION_TEXT_ADD_ANNOTATION);
}

void TextOperationWidget::SlotBtnAddBookMarkClicked()
{
    sendMsgAndHide(MSG_OPERATION_TEXT_ADD_BOOKMARK);
}

void TextOperationWidget::sendMsgAndHide(const int &msgType)
{
    sendMsg(msgType);
    this->hide();
}

int TextOperationWidget::dealWithData(const int &, const QString &)
{
    return 0;
}

void TextOperationWidget::initWidget()
{
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setLayout(layout);

    createBtn(tr("copy"), SLOT(SlotBtnCopyClicked()));
    m_pLightedWidget = new LightedWidget;
    layout->addWidget(m_pLightedWidget);

    createBtn(tr("remove high lighted"), SLOT(SlotBtnRemoveHighLightedClicked()));
    createBtn(tr("add annotation"), SLOT(SlotBtnAddAnnotationClicked()));
    createBtn(tr("add book mark"), SLOT(SlotBtnAddBookMarkClicked()));
}