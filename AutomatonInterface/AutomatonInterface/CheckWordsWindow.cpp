#include "CheckWordsWindow.h"

CheckWordsWindow::CheckWordsWindow(const std::vector<std::pair<QString, QString>>& words, QWidget* parent)
    : QMainWindow{ parent }
    , m_words{ words }
{
    resize(300, 200);
    setAttribute(Qt::WA_DeleteOnClose);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);

    m_scrollWidget = new QWidget(m_scrollArea);
    m_layout = new QVBoxLayout(m_scrollWidget);
    m_scrollWidget->setLayout(m_layout);

    for (const auto& pair : words)
    {
        QLabel* label = new QLabel(pair.first + pair.second);
        m_layout->addWidget(label);
    }

    m_scrollArea->setWidget(m_scrollWidget);

    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setCentralWidget(m_scrollArea);
}

CheckWordsWindow::~CheckWordsWindow()
{
}
